#include <Interpreters/FindIdentifierBestTableVisitor.h>
#include <Interpreters/IdentifierSemantic.h>


namespace DB
{

FindIdentifierBestTableData::FindIdentifierBestTableData(const std::vector<TableWithColumnNames> & tables_)
    : tables(tables_)
{
}

void FindIdentifierBestTableData::visit(ASTIdentifier & identifier, ASTPtr &)
{
    const DatabaseAndTableWithAlias * best_table = nullptr;

    if (!identifier.compound())
    {
        for (const auto & table_names : tables)
        {
            if (std::find(table_names.second.begin(), table_names.second.end(), identifier.name) != table_names.second.end())
            {
                // TODO: make sure no collision ever happens
                if (!best_table)
                    best_table = &table_names.first;
            }
        }
    }
    else
    {
        size_t best_table_pos = 0;
        if (IdentifierSemantic::chooseTable(identifier, tables, best_table_pos))
            best_table = &tables[best_table_pos].first;
    }

    identifier_table.emplace_back(&identifier, best_table);
}

}
