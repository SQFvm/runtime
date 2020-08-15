
#ifndef DRIVER_HH_
#define DRIVER_HH_

#include "astnode.h"


#include <string>
#include <iostream>
#include <fstream>


namespace parse
{
    /// Forward declarations of classes
    class Parser;
    class Scanner;
    class location;

    class Driver
    {
        public:
            ::sqf::sqc::astnode root;
            Driver();
            ~Driver();

            int parse();
            int parse_file(std::string& path);

            void reset();


        private:
            Scanner*      scanner_;
            Parser*       parser_;
            location*     location_;
            int           error_;

            /// Allows Parser and Scanner to access private attributes
            /// of the Driver class
            friend class  Parser;
            friend class  Scanner;
    };
}

#endif /* !DRIVER_HH_ */

