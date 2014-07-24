#pragma once
#include <vector>
#include <mutex>
#include <memory>
#include <boost/noncopyable.hpp>
#include <tuple>

namespace mandelbrot {

namespace split
{

struct Limits
{
    unsigned left, right, top, bottom;
};

class DataSplit;
class SimpleDataSplit;
class PuzzleDataSplit;

enum class DataSplitType { Simple, Puzzle };

class DataSplitFactory
{
public:
    template <typename... Args>
    static std::unique_ptr<DataSplit> create(DataSplitType type, Args&&... args)
    {
        switch (type) {
            case DataSplitType::Simple:
                return std::unique_ptr<DataSplit>(new SimpleDataSplit(args...));
            case DataSplitType::Puzzle:
                return std::unique_ptr<DataSplit>(new PuzzleDataSplit(args...));
        }

        throw std::runtime_error("No such type");
    }
};

class IDataSplit
{
public:
    typedef std::unique_ptr<Limits> type;

    virtual type next()=0;
    virtual void reset() =0;
};

//thread-safe modifiers
class DataSplit : public IDataSplit
{
public:
    std::unique_ptr<Limits> next() final;
    void reset() final;

protected:
    DataSplit(unsigned w, unsigned h) : limits_(), position_(limits_.end()), width_(w), height_(h) {};

    virtual void doit() =0;

    std::vector<Limits> limits_;
    decltype(limits_)::iterator position_;
    unsigned width_, height_;
};

class SimpleDataSplit : public DataSplit
{
public:
    SimpleDataSplit(unsigned w, unsigned h);

private:
    void doit() final;

    double percentage_;
    bool vertical_;
};

class PuzzleDataSplit : public DataSplit
{
public:
    PuzzleDataSplit(unsigned w, unsigned h);

private:
    void doit() final;

    double percentage_;
};

} //namespace split

} //namespace mandelbrot