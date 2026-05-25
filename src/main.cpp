#include "./ECS/Components.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits.h>

flecs::world ecs;

struct CompanyStruct
{
    unsigned long int uid;
    std::string name;
    std::string child_of;
    BalanceSheet balance_sheet;
    GeographicalPosition geoposition;
};

struct AtoBProductFlow
{
    CompanyStruct* CompanyA;
    CompanyStruct* CompanyB;
    ProductService* product;
};

std::vector<unsigned long int> CompaniesUid = {};

unsigned long int generate_random_unsigned_long_int()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long int> distrib(0, 100000000000);
    unsigned long int randomValue = distrib(gen);
    return randomValue;
}

template<typename T>
bool IsElemIntoVect(std::vector<T> v, T el)
{
    auto it = std::find(v.begin(), v.end(), el);
    if (it != v.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GetCompaniesInfo(std::vector<CompanyStruct>& CompaniesList, std::vector<AtoBProductFlow>& SCRelations)
{
    CompanyStruct Company1;
    long unsigned int uid1 = generate_random_unsigned_long_int();
    //long unsigned int uid1 = 1001;
    if (!IsElemIntoVect<long unsigned int>(CompaniesUid, uid1))
    {
        Company1.uid = uid1;
        CompaniesUid.push_back(uid1);
    }
    Company1.name = "Holding_MegaCorp";
    Company1.child_of = "";
    Company1.balance_sheet.capital = 1000000.0;
    Company1.balance_sheet.income = 50000.0;
    Company1.geoposition.latitude = 45.4642;
    Company1.geoposition.longitude = 9.1900;
    CompaniesList.push_back(Company1);

    CompanyStruct Company2;
    long unsigned int uid2 = generate_random_unsigned_long_int();
    //long unsigned int uid2 = 1002;
    if (!IsElemIntoVect<long unsigned int>(CompaniesUid, uid2))
    {
        Company2.uid = uid2;
        CompaniesUid.push_back(uid2);
    }
    Company2.name = "MegaCorpProductionBranch";
    Company2.child_of = "Holding_MegaCorp";
    Company2.balance_sheet.capital = 250000.0;
    Company2.balance_sheet.income = 12000.0;
    Company2.geoposition.latitude = 45.0703;
    Company2.geoposition.longitude = 7.6869;
    CompaniesList.push_back(Company2);

    CompanyStruct Company3;
    long unsigned int uid3 = generate_random_unsigned_long_int();
    //long unsigned int uid3 = 1003;
    if (!IsElemIntoVect<long unsigned int>(CompaniesUid, uid3))
    {
        Company3.uid = uid3;
        CompaniesUid.push_back(uid3);
    }
    Company3.name = "MegaCorpDistributionBranch";
    Company3.child_of = "Holding_MegaCorp";
    Company3.balance_sheet.capital = 150000.0;
    Company3.balance_sheet.income = 18000.0;
    Company3.geoposition.latitude = 44.4949;
    Company3.geoposition.longitude = 11.3426;
    CompaniesList.push_back(Company3);

    AtoBProductFlow CompanyAToCompanyB_PF_1;
    ProductService product_1;
    product_1.name = "Microchip";
    product_1.unit_price = 45.50;
    CompanyAToCompanyB_PF_1.CompanyA = &Company2;
    CompanyAToCompanyB_PF_1.CompanyB = &Company3;
    CompanyAToCompanyB_PF_1.product = &product_1;
    SCRelations.push_back(CompanyAToCompanyB_PF_1);
};

void EcsCreateCompaniesHierarchiesAndProductsSC()
{
    std::vector<CompanyStruct> CompaniesList;
    std::vector<AtoBProductFlow> SCRelations;
    GetCompaniesInfo(CompaniesList, SCRelations);

    std::cout << "EcsCreateCompaniesHierarchiesAndProductsSC:" << std::endl;

    // https://www.flecs.dev/flecs/md_docs_2EntitiesComponents.html
    for(CompanyStruct Company : CompaniesList)
    {
        ecs_entity_t e_id = Company.uid;
        std::cout << "e_id= " << e_id << std::endl;
        flecs::entity e = ecs.make_alive(e_id);
        std::cout << "ecs_is_valid(ecs, e)= " << ecs_is_valid(ecs, e) << std::endl;
        std::string company_name = Company.name;
        e.set_name(company_name.c_str());
        //flecs::entity e = ecs.entity(company_name.c_str());
        e.set<BalanceSheet>({Company.balance_sheet.capital, Company.balance_sheet.income});
        e.set<GeographicalPosition>({Company.geoposition.latitude, Company.geoposition.longitude});
        std::string parent_company = Company.child_of;
        if (parent_company != "")
        {
            const char* parent_company_c = parent_company.c_str();
            flecs::entity parent = ecs.lookup(parent_company_c); // Look the parent by name_c
            ecs_entity_t parent_id = parent.id(); // Get the parent_id
            e.child_of(parent); // Set the entity as child of the parent
        }
        std::cout << std::endl;
    }

    ecs.each([](flecs::entity e, GeographicalPosition& gp, BalanceSheet& bs)
    {
        std::cout << "e= " << e << std::endl;
        std::cout << "latitude= " << gp.latitude << " , longitude= " << gp.longitude <<std::endl;
        std::cout << "capital= " << bs.capital << " , income= " << bs.income << std::endl;
        std::cout << "parent= " << e.parent() << std::endl;
    });

}


int main(int, char *[])
{
    EcsCreateCompaniesHierarchiesAndProductsSC();

    return 0;
}
