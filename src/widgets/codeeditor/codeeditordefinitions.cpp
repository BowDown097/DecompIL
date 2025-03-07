#include "codeeditordefinitions.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>

KSyntaxHighlighting::Definition& CodeEditorDefinitions::CILDefinition()
{
    static KSyntaxHighlighting::Definition def = repository().definitionForName("CIL");
    return def;
}

KSyntaxHighlighting::Definition& CodeEditorDefinitions::CSharpDefinition()
{
    static KSyntaxHighlighting::Definition def = repository().definitionForName("C#");
    return def;
}

KSyntaxHighlighting::Repository& CodeEditorDefinitions::repository()
{
    static KSyntaxHighlighting::Repository repo;
    return repo;
}

void CodeEditorDefinitions::addTypes(const QList<NativeTypes::AssemblyTypeMetadata>& types)
{
    QStringList csList = CSharpDefinition().keywordList("types");
    QStringList cilList = CILDefinition().keywordList("type-list");

    for (const NativeTypes::AssemblyTypeMetadata& type : types)
    {
        if (s_customTypes.insert(type).second)
        {
            csList.append(type.name);
            cilList.append(type.name);
        }
    }

    CSharpDefinition().setKeywordList("types", csList);
    CILDefinition().setKeywordList("type-list", cilList);
}

void CodeEditorDefinitions::removeTypes(const QList<NativeTypes::AssemblyTypeMetadata>& types)
{
    QStringList csList = CSharpDefinition().keywordList("types");
    QStringList cilList = CILDefinition().keywordList("type-list");

    for (const NativeTypes::AssemblyTypeMetadata& type : types)
    {
        if (s_customTypes.erase(type))
        {
            csList.removeOne(type.name);
            cilList.removeOne(type.name);
        }
    }

    CSharpDefinition().setKeywordList("types", csList);
    CILDefinition().setKeywordList("type-list", cilList);
}

CodeEditorDefinitions::TypeRange CodeEditorDefinitions::match(const QString& name)
{
    auto [begin, end] = s_customTypes.equal_range(name);
    return std::ranges::subrange(begin, end);
}
