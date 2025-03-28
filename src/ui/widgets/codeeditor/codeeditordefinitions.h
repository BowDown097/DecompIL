#pragma once
#include "interface/types/native/assemblytypemetadata.h"
#include <set>

namespace KSyntaxHighlighting { class Definition; class Repository; class Theme; }

class CodeEditorDefinitions
{
    friend class CodeEditor;
public:
    using TypeRange = std::ranges::subrange<
        std::set<NativeTypes::AssemblyTypeMetadata>::iterator,
        std::set<NativeTypes::AssemblyTypeMetadata>::iterator>;

    static KSyntaxHighlighting::Theme defaultTheme();
    static QList<KSyntaxHighlighting::Theme> themes();

    static void addTypes(const QList<NativeTypes::AssemblyTypeMetadata>& types);
    static void removeTypes(const QList<NativeTypes::AssemblyTypeMetadata>& types);

    static TypeRange match(const QString& name);
private:
    inline static std::set<NativeTypes::AssemblyTypeMetadata, std::less<>> s_customTypes;

    static KSyntaxHighlighting::Definition& CILDefinition();
    static QStringList& CILTypeList();

    static KSyntaxHighlighting::Definition& CSharpDefinition();
    static QStringList& CSharpTypeList();

    static const KSyntaxHighlighting::Repository& repository();

    static void addType(const NativeTypes::AssemblyTypeMetadata& type);
    static void removeType(const NativeTypes::AssemblyTypeMetadata& type);
};
