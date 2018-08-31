using System;

namespace DebuggerCLI
{
	public interface ICommandHandlerItem
	{
		string ShortName { get; }
		string Name { get; }
		string Description { get; }
		string Details { get; }
		void Execute(params string[] items);
	}
	public class CommandHandlerItem : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke();
		}
		public CommandHandlerItem SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01)))
			);
		}
		public CommandHandlerItem<T01> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02)))
			);
		}
		public CommandHandlerItem<T01, T02> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03)))
			);
		}
		public CommandHandlerItem<T01, T02, T03> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08, T09> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08))),
				items.Length <  9 ? default(T09) : (T09)Convert.ChangeType(items[ 8], Type.GetTypeCode(typeof(T09)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08))),
				items.Length <  9 ? default(T09) : (T09)Convert.ChangeType(items[ 8], Type.GetTypeCode(typeof(T09))),
				items.Length < 10 ? default(T10) : (T10)Convert.ChangeType(items[ 9], Type.GetTypeCode(typeof(T10)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08))),
				items.Length <  9 ? default(T09) : (T09)Convert.ChangeType(items[ 8], Type.GetTypeCode(typeof(T09))),
				items.Length < 10 ? default(T10) : (T10)Convert.ChangeType(items[ 9], Type.GetTypeCode(typeof(T10))),
				items.Length < 11 ? default(T11) : (T11)Convert.ChangeType(items[10], Type.GetTypeCode(typeof(T11)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08))),
				items.Length <  9 ? default(T09) : (T09)Convert.ChangeType(items[ 8], Type.GetTypeCode(typeof(T09))),
				items.Length < 10 ? default(T10) : (T10)Convert.ChangeType(items[ 9], Type.GetTypeCode(typeof(T10))),
				items.Length < 11 ? default(T11) : (T11)Convert.ChangeType(items[10], Type.GetTypeCode(typeof(T11))),
				items.Length < 12 ? default(T12) : (T12)Convert.ChangeType(items[11], Type.GetTypeCode(typeof(T12)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08))),
				items.Length <  9 ? default(T09) : (T09)Convert.ChangeType(items[ 8], Type.GetTypeCode(typeof(T09))),
				items.Length < 10 ? default(T10) : (T10)Convert.ChangeType(items[ 9], Type.GetTypeCode(typeof(T10))),
				items.Length < 11 ? default(T11) : (T11)Convert.ChangeType(items[10], Type.GetTypeCode(typeof(T11))),
				items.Length < 12 ? default(T12) : (T12)Convert.ChangeType(items[11], Type.GetTypeCode(typeof(T12))),
				items.Length < 13 ? default(T13) : (T13)Convert.ChangeType(items[12], Type.GetTypeCode(typeof(T13)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08))),
				items.Length <  9 ? default(T09) : (T09)Convert.ChangeType(items[ 8], Type.GetTypeCode(typeof(T09))),
				items.Length < 10 ? default(T10) : (T10)Convert.ChangeType(items[ 9], Type.GetTypeCode(typeof(T10))),
				items.Length < 11 ? default(T11) : (T11)Convert.ChangeType(items[10], Type.GetTypeCode(typeof(T11))),
				items.Length < 12 ? default(T12) : (T12)Convert.ChangeType(items[11], Type.GetTypeCode(typeof(T12))),
				items.Length < 13 ? default(T13) : (T13)Convert.ChangeType(items[12], Type.GetTypeCode(typeof(T13))),
				items.Length < 14 ? default(T14) : (T14)Convert.ChangeType(items[13], Type.GetTypeCode(typeof(T14)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08))),
				items.Length <  9 ? default(T09) : (T09)Convert.ChangeType(items[ 8], Type.GetTypeCode(typeof(T09))),
				items.Length < 10 ? default(T10) : (T10)Convert.ChangeType(items[ 9], Type.GetTypeCode(typeof(T10))),
				items.Length < 11 ? default(T11) : (T11)Convert.ChangeType(items[10], Type.GetTypeCode(typeof(T11))),
				items.Length < 12 ? default(T12) : (T12)Convert.ChangeType(items[11], Type.GetTypeCode(typeof(T12))),
				items.Length < 13 ? default(T13) : (T13)Convert.ChangeType(items[12], Type.GetTypeCode(typeof(T13))),
				items.Length < 14 ? default(T14) : (T14)Convert.ChangeType(items[13], Type.GetTypeCode(typeof(T14))),
				items.Length < 15 ? default(T15) : (T15)Convert.ChangeType(items[14], Type.GetTypeCode(typeof(T15)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
	public class CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16> : ICommandHandlerItem
	{
		public string Details { get; private set; }
		public string ShortName { get; }
		public string Name { get; }
		public string Description { get; }
		public Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16> ExecutingAction { get; private set; }
		public CommandHandlerItem(string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16> act)
		{
			this.Details = String.Empty;
			this.ShortName = String.Empty;
			this.Name = name.ToLowerInvariant();
			this.Description = description;
			this.ExecutingAction = act;
		}
		public CommandHandlerItem(string shortname, string name, string description, Action<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16> act)
		{
			this.ShortName = shortname.ToLowerInvariant();
			this.Name = name.ToLowerInvariant();
			this.Details = String.Empty;
			this.Description = description;
			this.ExecutingAction = act;
		}
		public void Execute(params string[] items)
		{
			this.ExecutingAction?.Invoke(
				items.Length <  1 ? default(T01) : (T01)Convert.ChangeType(items[ 0], Type.GetTypeCode(typeof(T01))),
				items.Length <  2 ? default(T02) : (T02)Convert.ChangeType(items[ 1], Type.GetTypeCode(typeof(T02))),
				items.Length <  3 ? default(T03) : (T03)Convert.ChangeType(items[ 2], Type.GetTypeCode(typeof(T03))),
				items.Length <  4 ? default(T04) : (T04)Convert.ChangeType(items[ 3], Type.GetTypeCode(typeof(T04))),
				items.Length <  5 ? default(T05) : (T05)Convert.ChangeType(items[ 4], Type.GetTypeCode(typeof(T05))),
				items.Length <  6 ? default(T06) : (T06)Convert.ChangeType(items[ 5], Type.GetTypeCode(typeof(T06))),
				items.Length <  7 ? default(T07) : (T07)Convert.ChangeType(items[ 6], Type.GetTypeCode(typeof(T07))),
				items.Length <  8 ? default(T08) : (T08)Convert.ChangeType(items[ 7], Type.GetTypeCode(typeof(T08))),
				items.Length <  9 ? default(T09) : (T09)Convert.ChangeType(items[ 8], Type.GetTypeCode(typeof(T09))),
				items.Length < 10 ? default(T10) : (T10)Convert.ChangeType(items[ 9], Type.GetTypeCode(typeof(T10))),
				items.Length < 11 ? default(T11) : (T11)Convert.ChangeType(items[10], Type.GetTypeCode(typeof(T11))),
				items.Length < 12 ? default(T12) : (T12)Convert.ChangeType(items[11], Type.GetTypeCode(typeof(T12))),
				items.Length < 13 ? default(T13) : (T13)Convert.ChangeType(items[12], Type.GetTypeCode(typeof(T13))),
				items.Length < 14 ? default(T14) : (T14)Convert.ChangeType(items[13], Type.GetTypeCode(typeof(T14))),
				items.Length < 15 ? default(T15) : (T15)Convert.ChangeType(items[14], Type.GetTypeCode(typeof(T15))),
				items.Length < 16 ? default(T16) : (T16)Convert.ChangeType(items[15], Type.GetTypeCode(typeof(T16)))
			);
		}
		public CommandHandlerItem<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16> SetDetails(params string[] details) { this.Details = String.Join("\n", details); return this; }
	}
}