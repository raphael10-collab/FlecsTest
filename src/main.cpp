#include "./ECS/Components.h"
#include <iostream>
#include <vector>

flecs::world ecs;

struct CompanyStruct
{
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

void GetCompaniesInfo(std::vector<CompanyStruct>& CompaniesList, std::vector<AtoBProductFlow>& SCRelations)
{
    CompanyStruct Company1;
    Company1.name = "Holding_MegaCorp";
    Company1.child_of = "";
    Company1.balance_sheet.capital = 1000000.0;
    Company1.balance_sheet.income = 50000.0;
    Company1.geoposition.latitude = 45.4642;
    Company1.geoposition.longitude = 9.1900;
    CompaniesList.push_back(Company1);

    CompanyStruct Company2;
    Company2.name = "MegaCorpProductionBranch";
    Company2.child_of = "Holding_MegaCorp";
    Company2.balance_sheet.capital = 250000.0;
    Company2.balance_sheet.income = 12000.0;
    Company2.geoposition.latitude = 45.0703;
    Company2.geoposition.longitude = 7.6869;
    CompaniesList.push_back(Company2);

    CompanyStruct Company3;
    Company3.name = "MegaCorpDistributionBranch";
    Company3.child_of = "Holding_MegaCorp";
    Company3.balance_sheet.capital = 150000.0;
    Company2.balance_sheet.income = 18000.0;
    Company2.geoposition.latitude = 44.4949;
    Company2.geoposition.longitude = 11.3426;
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

    // https://www.flecs.dev/flecs/md_docs_2EntitiesComponents.html
    for(CompanyStruct Company : CompaniesList)
    {
        std::string company_name = Company.name;
        flecs::entity e = ecs.entity(company_name.c_str());
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
    }

    ecs.each([](flecs::entity e, GeographicalPosition& gp, BalanceSheet& bs)
    {
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
