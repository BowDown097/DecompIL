#pragma once
#include "interface_types/native/assemblytypemetadata.h"
#include <QList>
#include <set>

namespace KSyntaxHighlighting { class Definition; class Repository; }

class CodeEditorDefinitions
{
public:
    using TypeRange = std::ranges::subrange<
        std::set<NativeTypes::AssemblyTypeMetadata>::iterator,
        std::set<NativeTypes::AssemblyTypeMetadata>::iterator>;

    static KSyntaxHighlighting::Definition& CILDefinition();
    static KSyntaxHighlighting::Definition& CSharpDefinition();
    static KSyntaxHighlighting::Repository& repository();

    static void addTypes(const QList<NativeTypes::AssemblyTypeMetadata>& types);
    static void removeTypes(const QList<NativeTypes::AssemblyTypeMetadata>& types);

    TypeRange match(const QString& name);
private:
    inline static std::set<NativeTypes::AssemblyTypeMetadata, std::less<>> s_customTypes;
};
