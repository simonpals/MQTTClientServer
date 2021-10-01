#include "parser.h"
#include "location.h"

namespace ldp {

ParserPtr ParserFactory::factoryMethod(ParserFactory::Format ver)
{
    if(ver != Format::Version1) {
        assert("Parser must be implemented for such format" && 0);
    }

    return std::make_shared<LocationParserV1>();
}

LocationImpl ALocationParser::parse(const std::string &topic,
                   const std::string &msg) const {
    if (this->mHandler) {
        return this->mHandler->parse(topic, msg);
    }

    return LocationImpl{};
}

LocationImpl LocationParserV1::parse(const std::string &topic,
                                     const std::string &msg) const
{
    const std::regex configTopicRegx{"(" + std::string(misc::configTopic) + ")(\\w+)"};
    const std::regex dataTopicRegx{"(" + std::string(misc::dataStrTopic) + ")(\\w+)/(\\w+)"};
    const std::regex dataRegexp{R"([^;\s][^\;]*[^;\s]*)"};

    std::string sessionID;

    std::smatch match;
    uint16_t iMatch=0;

    std::string topicStr = topic;
    std::string dataStr = msg;

    while (std::regex_search(topicStr, match, configTopicRegx)) {
        for (const auto& str : match) {
            if(iMatch++==2) sessionID = std::move(str);
        }
        topicStr = match.suffix();
    }

    if(!sessionID.empty()) {
        std::smatch match1;
        std::vector<std::string> confValues;

        while (std::regex_search(dataStr, match1, dataRegexp)) {
            for (const auto& str : match1) {
                confValues.emplace_back(str);
            }
            dataStr = match1.suffix();
        }

        if(confValues[0] != "1")
            return ALocationParser::parse(topic, msg);

        assert("Must be 7 items in config data" && confValues.size() == 7);

        LocationImpl::updateSharedData(sessionID, std::shared_ptr<misc::Region_t>(
                                           new misc::Region_t{misc::Point_t{ std::stof(confValues[1])
                                                                             ,std::stof(confValues[2])
                                                                             ,std::stof(confValues[3]) }
                                                              ,misc::Point_t{ std::stof(confValues[4])
                                                                              ,std::stof(confValues[5])
                                                                              ,std::stof(confValues[6]) }
                                           }));
        return LocationImpl{};
    }

    std::string id;
    std::smatch match2;

    while (std::regex_search(topicStr, match2, dataTopicRegx)) {
        for (const auto& str : match2) {
            if(iMatch==2) sessionID = std::move(str);
            if(iMatch==3) id = std::move(str);
            iMatch++;
        }
        topicStr = match2.suffix();
    }

    std::vector<std::string> values;
    std::smatch match3;

    while (std::regex_search(dataStr, match3, dataRegexp)) {
        for (const auto& str : match3) {
            values.emplace_back(str);
        }
        dataStr = match3.suffix();
    }

    if(values[0] != "1")
        return ALocationParser::parse(topic, msg);

    if(sessionID.empty() || id.empty() || values.size() != 5) {
        assert("Error during parsing in LocationParserV1" && 0);
        return LocationImpl{};
    }

    return  LocationImpl{ std::stoull(values[4])
                ,{ std::stof(values[1])
                    ,std::stof(values[2])
                    ,std::stof(values[3])
                 }
                ,id,sessionID
            };
}

}
