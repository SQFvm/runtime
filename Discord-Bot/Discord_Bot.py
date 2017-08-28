import discord
import asyncio
import os
import unidecode
import subprocess
from ctypes import *
import _ctypes

def init_libsqfvm():
    path = os.path.dirname(os.path.realpath(__file__))
    path = "{}/../SQF-VM/libsqfvm.so".format(path)
    print('Loading libsqfvm from {}'.format(path))
    global libsqfvm
    libsqfvm = CDLL(path)
    libsqfvm.start_program.restype = c_char_p
    libsqfvm.start_program.argtypes = [c_char_p]


class MyClient(discord.Client):
    async def on_ready(self):
        print('Logged in as')
        print(self.user.name)
        print(self.user.id)
        print('------------')
        self.allowsqf = True

    async def on_message(self, message):
        if message.content.startswith('<@{}>'.format(self.user.id)):
            if not self.allowsqf:
                await message.channel.send("Currently not possible.")
                return
            sqf = unidecode.unidecode(message.content.replace('<@{}>'.format(self.user.id), ""))
            result = libsqfvm.start_program(sqf.encode('utf-8').strip())
            if not result:
                result = '<EMPTY>'
            else:
                result = result.decode()
            tmp = await message.channel.send("```sqf\n{}```".format(result))
        elif message.content.startswith('!<@{}>'.format(self.user.id)):
            cmd = message.content.replace('!<@{}>'.format(self.user.id), "").strip()
            if cmd == 'REBUILD':
                tmp = await message.channel.send("```Freeing current...```")
                try:
                    self.allowsqf = False
                    _ctypes.dlclose(libsqfvm._handle)
                    await tmp.edit(content="```Pulling latest sources...```")
                    subprocess.call(['git', 'pull'])
                    await tmp.edit(content="```Building libsqfvm...```")
                    if subprocess.call(['make', 'all', '-C' ,'../SQF-VM/']):
                        await tmp.edit(content="```!BUILD FAILED!```")
                        return
                    await tmp.edit(content="```Loading libsqfvm...```")
                    init_libsqfvm()
                    await tmp.edit(content="```DONE!```")
                    self.allowsqf = True
                except:
                    await tmp.edit(content="```!FAILED!```")
            else:
                await message.channel.send("Unknown command `{}`".format(cmd))

            #await tmp.edit(content='You have {} messages.'.format(counter))
client = MyClient()
token = ""
with open('DISCORD.TOKEN', 'r') as file:
    token = file.read().strip()
init_libsqfvm()
print ('Using token --> {}'.format(token))
client.run(token)
