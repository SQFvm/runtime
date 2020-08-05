Dear Reader,

please ensure that the test-cases are:

* **Contain no pathing informations** Sadly this for the time being makes testing `#include` unless explicitly adjusted on every system the tests run in.
* **Double Checked with Arma** The arma preprocessor is not the same as (for example) the one included in GCC. Thus please assure that you tested your result, using arma.

thanks.

-----

Tests work by preprocessing a `.sqf` file, loading the related `.txt` file (same name, different extension). and performing a `isEqualTo` on them.