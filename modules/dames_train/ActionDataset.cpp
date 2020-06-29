#include "ActionDataset.h"
#include <iostream>
#include <QDir>
#include <QRegExp>
#include <StateSequence.h>

bool ActionDataset::load(const QString& path)
{
    std::multimap<Checkers::State, Checkers::State> seen;

    parseRootDirectory(path, seen);

    myStatesFrom.clear();
    myStatesTo.clear();

    {
        bool has_current_state_from = false;
        Checkers::State current_state_from;
        std::map<Checkers::State, int> current_states_to;

        bool go_on = true;
        std::multimap<Checkers::State, Checkers::State>::iterator it = seen.begin();
        while(go_on)
        {
            bool dump = false;
            bool initialize = false;
            bool accept = false;

            if(it == seen.end())
            {
                go_on = false;

                if(has_current_state_from)
                {
                    dump = true;
                }
            }
            else
            {
                accept = true;

                if(has_current_state_from)
                {
                    if(current_state_from != it->first)
                    {
                        dump = true;
                        initialize = true;
                    }
                }
                else
                {
                    initialize = true;
                }
            }

            if(dump)
            {
                myStatesFrom.emplace_back();
                myStatesFrom.back().state = current_state_from;
                myStatesFrom.back().offset = myStatesTo.size();
                myStatesFrom.back().count = 0;

                std::map<Checkers::State,int>::iterator it2;

                int count = 0;

                it2 = current_states_to.begin();
                while(it2 != current_states_to.end())
                {
                    count += it2->second;
                    it2++;
                }

                if(count == 0) throw std::runtime_error("internal error");

                it2 = current_states_to.begin();
                while(it2 != current_states_to.end())
                {
                    myStatesTo.emplace_back();
                    myStatesTo.back().state = it2->first;
                    myStatesTo.back().probability = float(it2->second) / float(count);

                    myStatesFrom.back().count++;
                    it2++;
                }
            }

            if(initialize)
            {
                has_current_state_from = true;

                current_state_from = it->first;

                current_states_to.clear();

                Checkers::ActionIterator ait;
                ait.init(current_state_from);

                Checkers::Action action_;
                Checkers::State state_;
                while(ait.next(current_state_from, action_, state_))
                {
                    current_states_to[state_] = 0.0;
                }
            }

            if(accept)
            {
                std::map<Checkers::State,int>::iterator it2 = current_states_to.find(it->second);

                if(it2 == current_states_to.end()) throw std::runtime_error("internal error");

                it2->second++;

                it++;
            }
        }
    }

    /*
    {
        std::vector<Example>::iterator it;
        for(it=myExamples.begin(); it!=myExamples.end(); it++)
        {
            std::cout << "FROM" << std::endl;
            std::cout << it->state_from.getSquareGrid() << std::endl;
            std::cout << "TO " << it->probability << std::endl;
            std::cout << it->state_to.getSquareGrid() << std::endl;
            std::cout << std::endl;
        }
    }
    */

    std::cout << "Loaded " << getNumExamples() << " samples." << std::endl;

    return true;
}

void ActionDataset::getExample(size_t index, Checkers::State& state_from, std::vector<Checkers::State>& state_to, std::vector<float>& transition_probability) const
{
    state_from = myStatesFrom[index].state;

    state_to.clear();
    transition_probability.clear();

    for(int i=0; i<myStatesFrom[index].count; i++)
    {
        const int offset = myStatesFrom[index].offset + i;
        state_to.push_back( myStatesTo[offset].state );
        transition_probability.push_back( myStatesTo[offset].probability );
    }
}

size_t ActionDataset::getNumExamples() const
{
    return myStatesFrom.size();
}

void ActionDataset::parseRootDirectory(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen)
{
    QRegExp r0("result_([0-9]*)");

    QDir dir;
    if(!dir.cd(path)) throw std::runtime_error("could not access directory");

    QStringList content = dir.entryList(QDir::Dirs);
    for(const QString& item : content)
    {
        if(r0.exactMatch(item))
        {
            parseSubdirectory(dir.absoluteFilePath(item), seen);
        }
    }
}

void ActionDataset::parseSubdirectory(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen)
{
    QDir dir;
    if(!dir.cd(path)) throw std::runtime_error("could not access directory");

    const QString trial = dir.absoluteFilePath("log.txt");
    if(QFileInfo(trial).isFile())
    {
        parseFile(trial, seen);
    }
    else
    {
        QRegExp r1("log_([0-9]*).txt");

        QStringList content = dir.entryList(QDir::Files);
        for(const QString& item : content)
        {
            if(r1.exactMatch(item))
            {
                parseFile(dir.absoluteFilePath(item), seen);
            }
        }
    }
}

void ActionDataset::parseFile(const QString& path, std::multimap<Checkers::State, Checkers::State>& seen)
{
    std::vector<Checkers::State> sequence;
    const bool ok = StateSequence::load(path.toStdString(), sequence);

    if(!ok) throw std::runtime_error("could not load state sequence");

    if(sequence.size() >= 2)
    {
        int offset = (sequence.front().isMyTurn()) ? 1 : 0;

        while(offset+3 < sequence.size()) // FIXME we do not take the last one for now.
        //while(offset+1 < sequence.size())
        {
            const Checkers::State& s0 = sequence[offset];
            const Checkers::State& s1 = sequence[offset+1];

            if(s0.isMyTurn() || !s1.isMyTurn()) throw std::runtime_error("bad log file");

            seen.insert( std::pair<Checkers::State, Checkers::State>(s0, s1) );

            offset += 2;
        }
    }
}

