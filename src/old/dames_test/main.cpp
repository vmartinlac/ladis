#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <QtTest/QtTest>
#include <Checkers.h>
#include <GraphVizHook.h>
#include <StateSequence.h>

class DamesTest : public QObject
{
    Q_OBJECT

private slots:

    void testEqualityOperators()
    {
        int tmp[Checkers::N];

        tmp[0] = 0;
        tmp[1] = 1;
        Checkers::Action a0;
        a0.set(1, tmp);

        tmp[0] = 1;
        tmp[1] = 2;
        Checkers::Action a1;
        a1.set(1, tmp);

        QVERIFY(a0 == a0);
        QVERIFY(! (a0 != a0) );

        QVERIFY(a0 != a1);
        QVERIFY(! (a0 == a1) );
    }

    void testIndices()
    {
        QVERIFY( Checkers::flatIndexToGridIndex(0) == 1);
        QVERIFY( Checkers::flatIndexToGridIndex(4) == 9);
        QVERIFY( Checkers::flatIndexToGridIndex(6) == 12);
        QVERIFY( Checkers::flatIndexToGridIndex(13) == 27);

        QVERIFY( Checkers::getReachable(6, Checkers::NEIGHBOR_TOP_RIGHT) == 10 );
        QVERIFY( Checkers::getReachable(6, Checkers::NEIGHBOR_BOTTOM_RIGHT) == 0 );
        QVERIFY( Checkers::getReachable(6, Checkers::NEIGHBOR_TOP_LEFT) == 11 );
        QVERIFY( Checkers::getReachable(6, Checkers::NEIGHBOR_BOTTOM_LEFT) == 1 );
    }

    /*
    void testTmp()
    {
        const char* grid =
            " o o o o o\n"
            "o o o o o \n"
            " o . . o o\n"
            "o o . . . \n"
            " . . . . .\n"
            "o . . . . \n"
            " . . p . o\n"
            "p p . . p \n"
            " p . . p p\n"
            "p p p p p \n";

        Checkers::State s;
        s.setSquareGrid(grid);
        s.setMyTurn(true);

        Checkers::ActionIterator it;
        it.init(s);

        Checkers::Action a_;
        Checkers::State s_;
        while(it.next(s, a_, s_))
        {
            std::cout << a_.getTrajectory(0) << " -> " << a_.getTrajectory(1) << std::endl;
            std::cout << s_.getSquareGrid() << std::endl;
            std::cout << std::endl;
        }
    }
    */

    void testSerialize()
    {
        const std::string filename = "sequence.txt";
        std::vector<Checkers::State> sequence0;
        std::vector<Checkers::State> sequence1;

        sequence0.emplace_back();
        sequence0.back().setSquareGrid(
            " o o o o o\n"
            "o o o o o \n"
            " o o o o o\n"
            "P o o o o \n"
            " . . . . .\n"
            ". . . . . \n"
            " p p p p p\n"
            "p O p p p \n"
            " p p p p p\n"
            "p p p p p \n");
        sequence0.back().setMyTurn(true);

        sequence0.emplace_back();
        sequence0.back().setSquareGrid(
            " o . o . o\n"
            ". o . o . \n"
            " . o . o .\n"
            "P . o . o \n"
            " . . . . .\n"
            ". . . . . \n"
            " p . p . p\n"
            ". O . p . \n"
            " . p . p .\n"
            "p . p . p \n");
        sequence0.back().setMyTurn(true);

        StateSequence::save(sequence0, filename);
        StateSequence::load(filename, sequence1);

        QVERIFY(sequence0.size() == sequence1.size());
        QVERIFY( std::equal(sequence0.begin(), sequence0.end(), sequence1.begin()) );
    }

    void testPicture()
    {
        Checkers::State s;
        s.setSquareGrid(
            " o o o o o\n"
            "o o o o o \n"
            " o o o o o\n"
            "o o o o o \n"
            " . . . . .\n"
            ". . . . . \n"
            " p p p p p\n"
            "p p p p p \n"
            " p p p p p\n"
            "p p p p p \n");
        s.setMyTurn(true);

        GraphVizHook hook;

        Checkers::Solver solver;
        solver.resetHook(&hook);
        Checkers::UtilityFunction utility;

        Checkers::Action action;
        Checkers::State resulting_state;

        const bool ok = solver.solve(s, action, resulting_state, utility, 3);

        QVERIFY(ok);
    }

    void testSolver()
    {
        struct Point
        {
            std::string grid;
            int depth;
            std::vector<int> move;
        };

        std::vector<Point> points;

        {
            points.emplace_back();
            points.back().grid =
                " . . . . .\n"
                ". . . . . \n"
                " . . . O .\n"
                ". . . . . \n"
                " . . . . .\n"
                ". . . o . \n"
                " . . . p .\n"
                ". o . . . \n"
                " . p . . .\n"
                ". . . . p \n";
            points.back().depth = 7;
            points.back().move.assign({8, 19});
        }

        {
            points.emplace_back();
            points.back().grid =
                " . . . . .\n"
                ". . . . . \n"
                " . . p . .\n"
                ". . . . . \n" // 30 - 34
                " . . o . .\n" // 25 - 29
                ". . . . . \n" // 20 - 24
                " . . o o .\n" // 15 - 19
                ". . . p . \n" // 10 - 14
                " . . . . .\n" // 5 - 9
                ". . . . . \n"; // 0 - 4
            points.back().depth = 7;
            points.back().move.assign({11, 22, 31});
        }

        Checkers::Solver solver;
        Checkers::UtilityFunction utility;

        for(const Point& pt : points)
        {
            Checkers::State state;
            state.setSquareGrid(pt.grid.c_str());
            state.setMyTurn(true);

            Checkers::Action action;
            Checkers::State newstate;

            const bool ok = solver.solve(state, action, newstate, utility, pt.depth);

            QVERIFY(ok);

            if(ok)
            {
                //std::cout << newstate.getSquareGrid() << std::endl;

                QVERIFY(action.getNumMoves()+1 == pt.move.size());

                if(action.getNumMoves()+1 == pt.move.size())
                {
                    int k = 0;
                    for(int m : pt.move)
                    {
                        QVERIFY(m == action.getTrajectory(k));
                        k++;
                    }
                }
            }
        }
    }

    void testActionIterator()
    {
        Checkers::State s;

        s.setMyTurn(true);
        s.setSquareGrid(
            " . . . . .\n"
            ". . . . . \n"
            " . . p . .\n"
            ". . . . . \n" // 30 - 34
            " . . o . .\n" // 25 - 29
            ". . . . . \n" // 20 - 24
            " . . o o .\n" // 15 - 19
            ". . . p . \n" // 10 - 14
            " . . . . .\n" // 5 - 9
            ". . . . . \n"); // 0 - 4

        Checkers::ActionIterator it;
        it.init(s);

        Checkers::Action a;
        Checkers::State ss;
        while(it.next(s, a, ss))
        {
            //std::cout << ss.getSquareGrid() << std::endl;
        }
    }
};

QTEST_GUILESS_MAIN(DamesTest)
#include "main.moc"


