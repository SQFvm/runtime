# Implementation Requirements for classes

| Level  |      Applies To      |                          Requirement                         |
| ------ | -------------------- | ------------------------------------------------------------ |
|  MUST  | `sqf::runtime::type` | MUST reside in namespace `sqf::types` or lower               |
|        | `sqf::runtime::data` |                                                              |
| SHOULD |          *           | SHOULD be declared in-header as much as possible and viable. |
|  MUST  | `sqf::runtime::type` | MUST be prefixed with `t_`                                   |
|  MUST  | `sqf::runtime::data` | MUST be prefixed with `d_`                                   |
|  MUST  | `sqf::runtime::data` | MUST be contained with a corresponding `sqf::runtime::type`  |
| SHOULD | `sqf::runtime::data` | SHOULD provide operator overloads for `sqf::runtime::value`  |