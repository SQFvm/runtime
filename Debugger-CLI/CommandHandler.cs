using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace DebuggerCLI
{
    public class CommandHandler
    {
        public IEnumerable<ICommandHandlerItem> Commands => this._Commands;
        private readonly List<ICommandHandlerItem> _Commands;
        public CommandHandler()
        {
            this._Commands = new List<ICommandHandlerItem>()
            {
                new CommandHandlerItem("?", "Displays this information.", this.HelpMethod),
                new CommandHandlerItem<string>("help", "Displays the detailed description for a command if available.", this.HelpDetailsMethod)
            };
        }
        private void HelpDetailsMethod(string name)
        {
            ICommandHandlerItem cmd = this._Commands.FirstOrDefault((it) => it.Name.Equals(name) || it.ShortName.Equals(name));
            Console.ForegroundColor = ConsoleColor.Yellow;
            if (cmd == null)
            {
                Console.WriteLine($"Could not find a command with the name '{name}'.");
            }
            else
            {
                name = GetHelpName(cmd);
                var maxname = name.Length;
                var output = $":{name}{new string(' ', maxname - name.Length)} - ";
                Console.Write(output);
                PrintAccording(output.Length, String.Join("\n", cmd.Description, cmd.Details), true);
            }
            Console.ResetColor();
        }
        private void HelpMethod()
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            var maxname = this._Commands.Max((it) => GetHelpName(it).Length);
            PrintAccording(0, "Using quotes (\") you can group stuff together. Escaping quotes is not possible yet.", false);
            foreach (var cmd in this._Commands)
            {
                var name = GetHelpName(cmd);
                var output = $":{name}{new string(' ', maxname - name.Length)} - ";
                Console.Write(output);
                PrintAccording(output.Length, cmd.Description, true);
            }
            Console.ResetColor();
        }

        private void PrintAccording(int leftspace, string content, bool startprinted)
        {
            var space = Console.WindowWidth - leftspace - 1 <= 0 ? 1 : Console.WindowWidth - leftspace - 1;

            var lines = content.Split('\n').Where((l) => !String.IsNullOrWhiteSpace(l));
            foreach (var line in lines)
            {
                var index = 0;
                while (index < line.Length)
                {
                    var substr = line.Substring(index, line.Length - index < space ? line.Length - index : space);
                    if (substr.Length == space)
                    {
                        var index2 = substr.LastIndexOf(' ');
                        if (index2 > 0)
                        {
                            substr = substr.Substring(0, index2);
                        }
                    }
                    index += substr.Length;
                    if (startprinted)
                    {
                        startprinted = false;
                    }
                    else
                    {
                        Console.Write(new string(' ', leftspace));
                    }
                    Console.WriteLine(substr.Trim());
                }
            }
        }
        private static string GetHelpName(ICommandHandlerItem cmd)
        {
            if (String.IsNullOrWhiteSpace(cmd.ShortName))
            {
                return cmd.Name;
            }
            else
            {
                var index = cmd.Name.IndexOf(cmd.ShortName);
                if (index == -1)
                {
                    return $"{cmd.Name} ({cmd.ShortName})";
                }
                else
                {
                    return $"{cmd.Name.Substring(0, index)}[{cmd.ShortName}]{cmd.Name.Substring(index + cmd.ShortName.Length)}";
                }
            }
        }

        public void Add(ICommandHandlerItem cmd)
        {
            if (this._Commands.Any((it) => it.Name.Equals(cmd.Name)))
            {
                throw new InvalidOperationException("Command already existing.");
            }
            this._Commands.Add(cmd);
        }

        public bool TryHandle(string input)
        {
            if (input.Length > 1 && input[0] == ':')
            {
                input = input.Substring(1);
                var index = input.IndexOf(' ');
                var command = (index == -1 ? input : input.Substring(0, index)).ToLowerInvariant();
                var content = index == -1 ? String.Empty : input.Substring(index + 1);
                foreach (var cmd in this._Commands)
                {
                    if (cmd.Name.Equals(command) || cmd.ShortName.Equals(command))
                    {
                        var values = Regex.Matches(content, @"[\""].+?[\""]|[^ ]+").Cast<Match>().Select(m =>
                        {
                            var val = m.Value.Trim();
                            if (val.Length >= 2 && val[0] == '"' && val[val.Length - 1] == '"')
                            {
                                return val.Substring(1, val.Length - 2);
                            }
                            return val;
                        }).ToArray();
                        try
                        {
                            cmd.Execute(values);
                        }
                        catch (Exception ex)
                        {
                            Console.ForegroundColor = ConsoleColor.Black;
                            Console.BackgroundColor = ConsoleColor.Red;
                            Console.Write("Execution failed:");
                            Console.WriteLine(ex.Message);
                            Console.WriteLine($"Use `:help {cmd.Name}` to get help on this command.");
                            Console.ResetColor();
                        }
                        return true;
                    }
                }
            }
            return false;
        }
    }
}
