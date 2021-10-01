#ifndef PARSER_H
#define PARSER_H

#include "pch.h"

namespace ldp {

class LocationImpl;

class ILocationParser
{
public:
    virtual ~ILocationParser() {}
    virtual LocationImpl parse(const std::string &topic,
                               const std::string &msg) const = 0;
    virtual std::shared_ptr<ILocationParser> setNextParser
                            (std::shared_ptr<ILocationParser> handler) = 0;
};

class ALocationParser : public ILocationParser
{
public:
    std::shared_ptr<ILocationParser> setNextParser
                            (std::shared_ptr<ILocationParser> handler)  override {
        mHandler = handler;
        return handler;
    }

    LocationImpl parse(const std::string &topic,
                       const std::string &msg) const override;
private:
    std::shared_ptr<ILocationParser> mHandler;
};

class LocationParserV1 : public ALocationParser
{
    LocationImpl parse(const std::string &topic,
                       const std::string &msg) const override;
};


using ParserPtr = std::shared_ptr<ILocationParser>;

class ParserFactory
{
public:
    enum class Format { Version1=0 };

    static ParserPtr factoryMethod(ParserFactory::Format ver);
};

}

#endif // PARSER_H
