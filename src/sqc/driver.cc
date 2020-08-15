#include "driver.hh"
#include "parser.hh"
#include "scanner.hh"

namespace parse
{
    Driver::Driver()
        : scanner_ (new Scanner()),
          parser_ (new Parser(*this)),
          location_ (new location())
    {
    }


    Driver::~Driver()
    {
        delete parser_;
        delete scanner_;
        delete location_;
    }


    void Driver::reset()
    {
        delete location_;
        location_ = new location();
    }

    int Driver::parse()
    {
        scanner_->switch_streams(&std::cin, &std::cerr);
        parser_->parse();
        return 0;
    }

    int Driver::parse_file (std::string& path)
    {
        std::ifstream s(path.c_str(), std::ifstream::in);
        scanner_->switch_streams(&s, &std::cerr);

        parser_->parse();

        s.close();

        return 0;
    }
}
