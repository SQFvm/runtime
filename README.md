## SQF-VM
__SQF-VM is a fully working and open-source Virtual Machine for the scripting language of the ArmA Games.__

[![GitHub issues](https://img.shields.io/github/issues/SQFvm/vm)](https://github.com/SQFvm/vm/issues)
[![GitHub forks](https://img.shields.io/github/forks/SQFvm/vm)](https://github.com/SQFvm/vm/network)
[![GitHub license](https://img.shields.io/badge/license-LGPLv3-blue)](https://raw.githubusercontent.com/SQFvm/vm/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/SQFvm/vm)](https://github.com/SQFvm/vm/stargazers)
[![Github All Releases](https://img.shields.io/github/downloads/SQFvm/vm/total)](https://github.com/SQFvm/vm/releases)
[![Build Status](https://github.com/SQFvm/runtime/workflows/CI/badge.svg)](https://github.com/SQFvm/runtime/workflows/CI)
[![Discord](https://img.shields.io/discord/365800217871908864)](https://discord.gg/vpdwJtG)

### Cloning
When cloning this repository, you have to do a recursive clone by doing the following: `git clone https://github.com/SQFvm/vm.git --recursive`.

If you already have cloned without the `--recursive`, you have to perform `git submodule init` followed by `git submodule update` in order to get all dependencies cloned.

### FAQ

> ##### How to use this thing?
> To start using the SQF-VM, you just have to double click the executable.
>  
> ##### What can it be used for (examples)?
> Most obvious example is: you are using a laptop while traveling and wanna test some SQF code.
> More advanced users might even use this for unit-testing their code.
>  
> ##### Are there command line parameters available to eg. use this for unit-testing?
> Yes, there are. To get all, start the tool via command line and the parameter -?
>  
> ##### How to use the debug interface?
> There is documentation available on the github page inside of Documentation/Debugger.
>  
> ##### What is supported?
> To check what currently is supported and what is not, you can use the `help__ "name"` command.
> If it outputs proper descriptions, the command is fully supported.
> If it does not, then the command is not yet implemented sadly.
>  
> ##### I found a bug, what shall i do now?
> Try to narrow down the problem as much as you can and then head over to the github issues section to report the bug.
>  
> ##### I got a question that is not answered in here, what to do?
> Ask your question in this thread or in the discord channel and i might add it to this list :)

### Links

[Website (Download)](https://x39.io/projects?project=SQF-VM)
[Discord Invite](https://discord.gg/vpdwJtG)

### Issue / Pull Request Template

A template on how to make Pull Requests can be found [here](https://github.com/SQFvm/vm/blob/master/PULL_REQUEST_TEMPLATE.md)

For Issues please use [this](https://github.com/SQFvm/vm/blob/master/ISSUE_TEMPLATE.md) template.

### Wiki / Documentation

WIP, for now please refer to the Discord.


### Diagnostics build flags

| Macro Name                                   | Prefix stdout                      | Description                                                                                      |
|----------------------------------------------|------------------------------------|--------------------------------------------------------------------------------------------------|
| `DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE` | `\x1B[94m[ASSEMBLY ASSERT]\033[0m` | Enables enhanced debug output to console that allows diagnosing issues related to the execution. |
| `DF__SQF_FILEIO__ALL_FILES`                  | `\x1B[94m[FILEIO-ALLFILES]\033[0m` | Outputs the files located of the `allFiles ARRAY` operation.                                     |
| `DF__SQF_PREPROC__TRACE_MACRO_RESOLVE`       | `\x1B[94m[PREPROCESSOR-RS]\033[0m` | Outputs every step during resolution of macro chains.                                            |
| `DF__SQF_PREPROC__TRACE_MACRO_PARSE`         | `\x1B[94m[PP-DEFINE-PARSE]\033[0m` | Outputs additional info during macro-parsing.                                                    |
| `DF__SQF_CONFIG__REPORT_PROGRESS_BY_LINE`    | `\x1B[94m[CNF-PARSE-PROGR]\033[0m` | Counts the lines in a config-to-parse and reports progress according to it to stdout.            |
| `DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG`         | `\x1B[94m[VALUE-SCOPE-DBG]\033[0m` | Every variable-change,-get,-set is logged to console.                                            |
| `DF__CLI_PRINT_INPUT_ARGS`                   | `\x1B[95m[CLI-INARG-PRINT]\033[0m` | Tells the CLI to print out the arguments received.                                               |
| `DF__CLI_PRINT_TCLAP_ARGUMENTS`              | `\x1B[95m[CLI-TCLAP-CNTNT]\033[0m` | Tells the CLI to print out the parsed arguments available.                                       |









