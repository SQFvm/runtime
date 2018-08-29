import discord
import asyncio
import os
import re
import subprocess
from ctypes import *
import _ctypes
import binascii


def init_libsqfvm():
    path = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
    path = "{}/sqfvm-cpp/libsqfvm.so".format(path)
    print('Loading libsqfvm from {}'.format(path))
    global libsqfvm
    libsqfvm = CDLL(path)
    libsqfvm.sqfvm_init.restype = None
    libsqfvm.sqfvm_init.argtypes = [c_ulonglong]
    libsqfvm.sqfvm_exec.restype = None
    libsqfvm.sqfvm_exec.argtypes = [c_char_p, c_char_p, c_size_t]
    libsqfvm.sqfvm_uninit.restype = None
    libsqfvm.sqfvm_uninit.argtypes = []
    libsqfvm.sqfvm_loadconfig.restype = None
    libsqfvm.sqfvm_loadconfig.argtypes = [c_char_p]
    print('Calling sqfvm_init')
    libsqfvm.sqfvm_init(1000000)
    print('Loading arma config')
    file = ""
    with open('arma.cpp', 'r') as file:
        file = file.read().strip()
    if file != "":
        print('calling sqfvm_loadconfig with arma.cpp')
        libsqfvm.sqfvm_loadconfig(file.encode('utf-8'))

def execsqf(txt, note):
    buffer = create_string_buffer(b'\000', 1990)
    print("Executing '{}' {}".format(txt, note))
    libsqfvm.sqfvm_exec(txt.encode('utf-8'), buffer, 1990)
    str = buffer.raw.decode('utf-8').strip()
    print(str)
    return str

class MyClient(discord.Client):
    async def on_ready(self):
        print('Logged in as')
        print(self.user.name)
        print(self.user.id)
        print('------------')
        self.allowsqf = True
        self.admins = [105784568346324992]

    async def on_message(self, message):
        if message.author.id == self.user.id:
            return
        if message.content.startswith('!') and message.author.id in self.admins:
            cmd = message.content[1:].strip()
            if cmd.lower() == 'rebuild':
                tmp = await message.channel.send("```Freeing current...```")
                try:
                    self.allowsqf = False
                    libsqfvm.sqfvm_uninit()
                    _ctypes.dlclose(libsqfvm._handle)
                    await tmp.edit(content="```Pulling latest sources...```")
                    subprocess.call(['git', 'pull'])
                    await tmp.edit(content="```Building libsqfvm...```")
                    if subprocess.call(['make', 'all', '-C' ,'../sqfvm-cpp/']):
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
                exit()
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
                    val = execsqf(message.content[6:-3], "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```{}```".format(val))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
                elif message.content.startswith('<@{}>'.format(self.user.id)):
                    val = execsqf(message.content.replace('<@{}>'.format(self.user.id), ""), self.outputbuffer, 1990, "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```sqf\n{}```".format(val))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
                else:
                    val = execsqf(message.content, self.outputbuffer, 1990, "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```sqf\n{}```".format(val))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
            else:
                if message.channel.name.startswith('sqf') and message.content.startswith('```sqf'):
                    val = execsqf(message.content[6:-3], "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```{}```".format(val))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
                elif message.content.startswith('<@{}>'.format(self.user.id)):
                    val = execsqf(message.content.replace('<@{}>'.format(self.user.id), ""), "from user {}#{}".format(message.author.name, message.author.id))
                    try:
                        tmp = await message.channel.send("```sqf\n{}```".format(val))
                    except Exception as e:
                        await message.channel.send("```!DISCORD ERROR!\n{}```".format(e))
client = MyClient()
token = ""
with open('DISCORD.TOKEN', 'r') as file:
    token = file.read().strip()
init_libsqfvm()
print ('Using token --> {}'.format(token))
client.run(token)

