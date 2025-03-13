#include "codeeditordefinitions.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>
#include <QApplication>
#include <QPalette>

KSyntaxHighlighting::Definition& CodeEditorDefinitions::CILDefinition()
{
    static KSyntaxHighlighting::Definition def = repository().definitionForName("Common Intermediate Language (CIL)");
    return def;
}

QStringList& CodeEditorDefinitions::CILTypeList()
{
    static QStringList list = CILDefinition().keywordList("type-list");
    return list;
}

KSyntaxHighlighting::Definition& CodeEditorDefinitions::CSharpDefinition()
{
    static KSyntaxHighlighting::Definition def = repository().definitionForName("C#");
    return def;
}

QStringList& CodeEditorDefinitions::CSharpTypeList()
{
    static QStringList list = CSharpDefinition().keywordList("types");
    return list;
}

KSyntaxHighlighting::Theme CodeEditorDefinitions::defaultTheme()
{
    return qApp->palette().color(QPalette::Base).lightness() < 128
        ? repository().defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
        : repository().defaultTheme(KSyntaxHighlighting::Repository::LightTheme);
}

const KSyntaxHighlighting::Repository& CodeEditorDefinitions::repository()
{
    static KSyntaxHighlighting::Repository repo;
    return repo;
}

QList<KSyntaxHighlighting::Theme> CodeEditorDefinitions::themes()
{
    return repository().themes();
}

void CodeEditorDefinitions::addType(const NativeTypes::AssemblyTypeMetadata& type)
{
    if (s_customTypes.insert(type).second)
    {
        CSharpTypeList().append(type.name);
        CILTypeList().append(type.name);
    }

    for (const NativeTypes::AssemblyTypeMetadata& nestedType : type.nestedTypes)
        addType(nestedType);
}

void CodeEditorDefinitions::addTypes(const QList<NativeTypes::AssemblyTypeMetadata>& types)
{
    for (const NativeTypes::AssemblyTypeMetadata& type : types)
        addType(type);
    CSharpDefinition().setKeywordList("types", CSharpTypeList());
    CILDefinition().setKeywordList("type-list", CILTypeList());
}

void CodeEditorDefinitions::removeType(const NativeTypes::AssemblyTypeMetadata& type)
{
    if (s_customTypes.erase(type))
    {
        CSharpTypeList().removeOne(type.name);
        CILTypeList().removeOne(type.name);
    }

    for (const NativeTypes::AssemblyTypeMetadata& nestedType : type.nestedTypes)
        removeType(nestedType);
}

void CodeEditorDefinitions::removeTypes(const QList<NativeTypes::AssemblyTypeMetadata>& types)
{
    for (const NativeTypes::AssemblyTypeMetadata& type : types)
        removeType(type);
    CSharpDefinition().setKeywordList("types", CSharpTypeList());
    CILDefinition().setKeywordList("type-list", CILTypeList());
}

CodeEditorDefinitions::TypeRange CodeEditorDefinitions::match(const QString& name)
{
    auto [begin, end] = s_customTypes.equal_range(name);
    return std::ranges::subrange(begin, end);
}
