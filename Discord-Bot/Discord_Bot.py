import discord
import asyncio
import os
import subprocess
from ctypes import *
import _ctypes

def init_libsqfvm():
    path = os.path.dirname(os.path.realpath(__file__))
    path = "{}/../SQF-VM/libsqfvm.so".format(path)
    print('Loading libsqfvm from {}'.format(path))
    global libsqfvm
    libsqfvm = CDLL(path)
    libsqfvm.start_program.restype = c_ubyte
    libsqfvm.start_program.argtypes = [c_wchar_p, c_ulong, c_wchar_p, c_size_t]

def execsqf(txt, buffer, bufferlen, note):
    print("Executing '{}' {}".format(txt, note))
    libsqfvm.start_program(txt, 1000000, None, 0)
    libsqfvm.start_program(txt, 1000000, buffer, bufferlen)
    #print("Result: {}", buffer.value.encode("utf-8"))

class MyClient(discord.Client):
    async def on_ready(self):
        print('Logged in as')
        print(self.user.name)
        print(self.user.id)
        print('------------')
        self.allowsqf = True
        self.outputbuffer = create_unicode_buffer(1990)
        self.admins = [105784568346324992]

    async def on_message(self, message):
        if message.author.id == self.user.id:
            return
        if message.content.startswith('!'):
            cmd = message.content[1:].strip()
            if cmd.lower() == 'rebuild' and message.author.id in self.admins:
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
                except Exception as e:
                    await tmp.edit(content="```!FAILED!\n{}```".format(e))
            elif cmd.lower() == 'quit' and message.author.id in self.admins:
                self.logout()
            elif cmd.lower() == 'help':
                tmp = await message.channel.send("```\nhelp - Displays this\nrebuild - Rebuilds the bot (requires being botadmin)\nquit - quits the bot, can be used for restart (requires being botadmin)```")
            else:
                await message.channel.send("Unknown command `{}`".format(cmd))
        else:
            if not self.allowsqf:
                await message.channel.send("Temporary Not Allowed.")
                return
            elif type(message.channel) is discord.DMChannel:
                if message.content.startswith('```sqf'):
                    execsqf(message.content[6:-3], self.outputbuffer, 1990, "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```{}```".format(self.outputbuffer.value.decode("utf-8")))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
                elif message.content.startswith('<@{}>'.format(self.user.id)):
                    execsqf(message.content.replace('<@{}>'.format(self.user.id), ""), self.outputbuffer, 1990, "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```sqf\n{}```".format(self.outputbuffer.value.decode("utf-8")))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
                else:
                    execsqf(message.content, self.outputbuffer, 1990, "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```sqf\n{}```".format(self.outputbuffer.value.decode("utf-8")))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
            else:
                if message.channel.name.startswith('sqf') and message.content.startswith('```sqf'):
                    execsqf(message.content[6:-3], self.outputbuffer, 1990, "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```{}```".format(self.outputbuffer.value.decode("utf-8")))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
                elif message.content.startswith('<@{}>'.format(self.user.id)):
                    execsqf(message.content.replace('<@{}>'.format(self.user.id), ""), self.outputbuffer, 1990, "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```sqf\n{}```".format(self.outputbuffer.value.decode("utf-8")))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
client = MyClient()
token = ""
with open('DISCORD.TOKEN', 'r') as file:
    token = file.read().strip()
init_libsqfvm()
print ('Using token --> {}'.format(token))
client.run(token)
