using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Discord;
using Discord.Net;
using Discord.WebSocket;

namespace Discord_Bot
{
    class Program
    {
        static void Main(string[] args)
        {
            new Program().Start().GetAwaiter().GetResult();
        }
        static Task TaskDone = Task.Run(() => { });
        private DiscordSocketClient Client;
        public async Task Start()
        {
            var config = ConfigXml.Load("config.xml");
            Client = new DiscordSocketClient();

            Console.WriteLine(string.Format("Logging in with token {0}", config.Token));
            await Client.LoginAsync(TokenType.Bot, config.Token);
            if(Client.LoginState == LoginState.LoggedOut)
            {
                Console.WriteLine("Login Failed");
                return;
            }
            Console.WriteLine("Login Success");
            Console.WriteLine("Starting ...");
            await Client.StartAsync();
            Console.WriteLine("Discord Bot Initialized!");
            Client.MessageReceived += Client_MessageReceived;
            Console.WriteLine("Starting Endless loop ...");
            await Task.Delay(-1);
        }

        private async Task Client_MessageReceived(SocketMessage arg)
        {
            if (arg.MentionedUsers.Any((u) => Client.CurrentUser.Id == u.Id))
            {
                Console.WriteLine(string.Format("{0}#{1}\t{2}", arg.Source, arg.Author.Discriminator, arg.Content.Replace("\n", "\\n")));
                string sqf = Regex.Replace(arg.Content, "<@[0-9]*>", string.Empty).Replace("```SQF", string.Empty).Replace("`", string.Empty).Trim();
                sqf = Regex.Replace(sqf, @"[^\u0000-\u007F]+", string.Empty);
                if(sqf.Length == 0)
                {
                    await arg.Channel.SendMessageAsync("ERROR: `empty message`");
                    return;
                }
                try
                {
                    string result = SQF_VM.StartProgram(sqf);
                    await arg.Channel.SendMessageAsync(string.Format("```sqf\n{0}```", string.IsNullOrWhiteSpace(result) ? "<EMPTY>" : result));
                }
                catch(Exception ex)
                {
                    await arg.Channel.SendMessageAsync(string.Format("ERROR: `{0}`", ex.Message));
                }
            }
            else if(arg.Author.Id == Client.CurrentUser.Id)
            {
                Console.WriteLine(string.Format("<SELF>\t{0}", arg.Content.Replace("\n", "\\n")));
            }
        }

        private static Task Client_Log(LogMessage arg)
        {
            Console.WriteLine(string.Format("{0}:{1}:{2}", arg.Source, arg.Severity, arg.Message));
            return TaskDone;
        }
    }
}
