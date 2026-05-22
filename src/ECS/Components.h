#include <string>
#include <flecs.h>

struct BalanceSheet
{
    double capital;
    double income;
};

struct GeographicalPosition
{
    double latitude;
    double longitude;
};

struct ProductService
{
    std::string name;
    double unit_price;
};

