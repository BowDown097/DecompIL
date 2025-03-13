#pragma once
#include <concepts>

struct DirectoryEntry
{
    int relativeVirtualAddress;
    int size;
};

enum class Characteristics : unsigned short
{
    RelocsStripped = 0x0001,         // Relocation info stripped from file.
    ExecutableImage = 0x0002,        // File is executable  (i.e. no unresolved external references).
    LineNumsStripped = 0x0004,       // Line numbers stripped from file.
    LocalSymsStripped = 0x0008,      // Local symbols stripped from file.
    AggressiveWSTrim = 0x0010,       // Aggressively trim working set
    LargeAddressAware = 0x0020,      // App can handle >2gb addresses
    BytesReversedLo = 0x0080,        // Bytes of machine word are reversed.
    Bit32Machine = 0x0100,           // 32 bit word machine.
    DebugStripped = 0x0200,          // Debugging info stripped from file in .DBG file
    RemovableRunFromSwap = 0x0400,   // If Image is on removable media, copy and run from the swap file.
    NetRunFromSwap = 0x0800,         // If Image is on Net, copy and run from the swap file.
    System = 0x1000,                 // System File.
    Dll = 0x2000,                    // File is a DLL.
    UpSystemOnly = 0x4000,           // File should only be run on a UP machine
    BytesReversedHi = 0x8000         // Bytes of machine word are reversed.
};

enum class CorFlags
{
    ILOnly = 0x00000001,
    Requires32Bit = 0x00000002,
    ILLibrary = 0x00000004,
    StrongNameSigned = 0x00000008,
    NativeEntryPoint = 0x00000010,
    TrackDebugData = 0x00010000,
    Prefers32Bit = 0x00020000
};

enum class DllCharacteristics : unsigned short
{
    /// <summary>
    /// Reserved.
    /// </summary>
    ProcessInit = 0x0001,

    /// <summary>
    /// Reserved.
    /// </summary>
    ProcessTerm = 0x0002,

    /// <summary>
    /// Reserved.
    /// </summary>
    ThreadInit = 0x0004,

    /// <summary>
    /// Reserved.
    /// </summary>
    ThreadTerm = 0x0008,

    /// <summary>
    /// Image can handle a high entropy 64-bit virtual address space.
    /// </summary>
    HighEntropyVirtualAddressSpace = 0x0020,

    /// <summary>
    /// DLL can move.
    /// </summary>
    DynamicBase = 0x0040,

    /// <summary>
    /// Code integrity checks are enforced.
    /// </summary>
    ForceIntegrity = 0x0080,

    /// <summary>
    /// Image is NX compatible.
    /// </summary>
    NxCompatible = 0x0100,

    /// <summary>
    /// Image understands isolation and doesn't want it.
    /// </summary>
    NoIsolation = 0x0200,

    /// <summary>
    /// Image does not use SEH.  No SE handler may reside in this image.
    /// </summary>
    NoSeh = 0x0400,

    /// <summary>
    /// Do not bind this image.
    /// </summary>
    NoBind = 0x0800,

    /// <summary>
    /// The image must run inside an AppContainer.
    /// </summary>
    AppContainer = 0x1000,

    /// <summary>
    /// Driver uses WDM model.
    /// </summary>
    WdmDriver = 0x2000,

    /// <summary>
    /// The image supports Control Flow Guard.
    /// </summary>
    ControlFlowGuard = 0x4000,

    /// <summary>
    /// The image is Terminal Server aware.
    /// </summary>
    TerminalServerAware = 0x8000
};

enum class Machine : unsigned short
{
    /// <summary>
    /// The target CPU is unknown or not specified.
    /// </summary>
    Unknown = 0x0000,

    /// <summary>
    /// Intel 386.
    /// </summary>
    I386 = 0x014C,

    /// <summary>
    /// MIPS little-endian WCE v2
    /// </summary>
    WceMipsV2 = 0x0169,

    /// <summary>
    /// Alpha
    /// </summary>
    Alpha = 0x0184,

    /// <summary>
    /// Hitachi SH3 little endian
    /// </summary>
    SH3 = 0x01a2,

    /// <summary>
    /// Hitachi SH3 DSP.
    /// </summary>
    SH3Dsp = 0x01a3,

    /// <summary>
    /// Hitachi SH3 little endian.
    /// </summary>
    SH3E = 0x01a4,

    /// <summary>
    /// Hitachi SH4 little endian.
    /// </summary>
    SH4 = 0x01a6,

    /// <summary>
    /// Hitachi SH5.
    /// </summary>
    SH5 = 0x01a8,

    /// <summary>
    /// ARM little endian
    /// </summary>
    Arm = 0x01c0,

    /// <summary>
    /// Thumb.
    /// </summary>
    Thumb = 0x01c2,

    /// <summary>
    /// ARM Thumb-2 little endian.
    /// </summary>
    ArmThumb2 = 0x01c4,

    /// <summary>
    /// Matsushita AM33.
    /// </summary>
    AM33 = 0x01d3,

    /// <summary>
    /// IBM PowerPC little endian.
    /// </summary>
    PowerPC = 0x01F0,

    /// <summary>
    /// PowerPCFP
    /// </summary>
    PowerPCFP = 0x01f1,

    /// <summary>
    /// Intel 64
    /// </summary>
    IA64 = 0x0200,

    /// <summary>
    /// MIPS
    /// </summary>
    MIPS16 = 0x0266,

    /// <summary>
    /// ALPHA64
    /// </summary>
    Alpha64 = 0x0284,

    /// <summary>
    /// MIPS with FPU.
    /// </summary>
    MipsFpu = 0x0366,

    /// <summary>
    /// MIPS16 with FPU.
    /// </summary>
    MipsFpu16 = 0x0466,

    /// <summary>
    /// Infineon
    /// </summary>
    Tricore = 0x0520,

    /// <summary>
    /// EFI Byte Code
    /// </summary>
    Ebc = 0x0EBC,

    /// <summary>
    /// AMD64 (K8)
    /// </summary>
    Amd64 = 0x8664,

    /// <summary>
    /// M32R little-endian
    /// </summary>
    M32R = 0x9041,

    /// <summary>
    /// ARM64
    /// </summary>
    Arm64 = 0xAA64,

    /// <summary>
    /// LOONGARCH32
    /// </summary>
    LoongArch32 = 0x6232,

    /// <summary>
    /// LOONGARCH64
    /// </summary>
    LoongArch64 = 0x6264,

    /// <summary>
    /// RISCV32
    /// </summary>
    RiscV32 = 0x5032,

    /// <summary>
    /// RISCV64
    /// </summary>
    RiscV64 = 0x5064,

    /// <summary>
    /// RISCV128
    /// </summary>
    RiscV128 = 0x5128
};

enum class PEMagic : unsigned short
{
    PE32 = 0x010B,
    PE32Plus = 0x020B
};

enum SectionCharacteristics : unsigned int
{
    TypeReg = 0x00000000,               // Reserved.
    TypeDSect = 0x00000001,             // Reserved.
    TypeNoLoad = 0x00000002,            // Reserved.
    TypeGroup = 0x00000004,             // Reserved.
    TypeNoPad = 0x00000008,             // Reserved.
    TypeCopy = 0x00000010,              // Reserved.

    ContainsCode = 0x00000020,               // Section contains code.
    ContainsInitializedData = 0x00000040,    // Section contains initialized data.
    ContainsUninitializedData = 0x00000080,  // Section contains uninitialized data.

    LinkerOther = 0x00000100,            // Reserved.
    LinkerInfo = 0x00000200,             // Section contains comments or some other type of information.
    TypeOver = 0x00000400,            // Reserved.
    LinkerRemove = 0x00000800,           // Section contents will not become part of image.
    LinkerComdat = 0x00001000,           // Section contents comdat.
    //                               0x00002000  // Reserved.
    MemProtected = 0x00004000,
    NoDeferSpecExc = 0x00004000,   // Reset speculative exceptions handling bits in the TLB entries for this section.
    GPRel = 0x00008000,               // Section content can be accessed relative to GP
    MemFardata = 0x00008000,
    MemSysheap = 0x00010000,
    MemPurgeable = 0x00020000,
    Mem16Bit = 0x00020000,
    MemLocked = 0x00040000,
    MemPreload = 0x00080000,

    Align1Bytes = 0x00100000,     //
    Align2Bytes = 0x00200000,     //
    Align4Bytes = 0x00300000,     //
    Align8Bytes = 0x00400000,     //
    Align16Bytes = 0x00500000,    // Default alignment if no others are specified.
    Align32Bytes = 0x00600000,    //
    Align64Bytes = 0x00700000,    //
    Align128Bytes = 0x00800000,   //
    Align256Bytes = 0x00900000,   //
    Align512Bytes = 0x00A00000,   //
    Align1024Bytes = 0x00B00000,  //
    Align2048Bytes = 0x00C00000,  //
    Align4096Bytes = 0x00D00000,  //
    Align8192Bytes = 0x00E00000,  //
    // Unused                     0x00F00000
    AlignMask = 0x00F00000,

    LinkerNRelocOvfl = 0x01000000,   // Section contains extended relocations.
    MemDiscardable = 0x02000000,     // Section can be discarded.
    MemNotCached = 0x04000000,       // Section is not cachable.
    MemNotPaged = 0x08000000,        // Section is not pageable.
    MemShared = 0x10000000,          // Section is shareable.
    MemExecute = 0x20000000,         // Section is executable.
    MemRead = 0x40000000,            // Section is readable.
    MemWrite = 0x80000000            // Section is writable.
};

enum class Subsystem : unsigned short
{
    Unknown = 0,                // Unknown subsystem.
    Native = 1,                 // Image doesn't require a subsystem.
    WindowsGui = 2,             // Image runs in the Windows GUI subsystem.
    WindowsCui = 3,             // Image runs in the Windows character subsystem.
    OS2Cui = 5,                 // image runs in the OS/2 character subsystem.
    PosixCui = 7,               // image runs in the Posix character subsystem.
    NativeWindows = 8,          // image is a native Win9x driver.
    WindowsCEGui = 9,           // Image runs in the Windows CE subsystem.
    EfiApplication = 10,        // Extensible Firmware Interface (EFI) application.
    EfiBootServiceDriver = 11,  // EFI driver with boot services.
    EfiRuntimeDriver = 12,      // EFI driver with run-time services.
    EfiRom = 13,                // EFI ROM image.
    Xbox = 14,                  // XBox system.
    WindowsBootApplication = 16 // Boot application.
};

enum class TypeAttributes
{
    //
    // Summary:
    //     LPTSTR is interpreted as ANSI.
    AnsiClass = 0,
    //
    // Summary:
    //     Specifies that class fields are automatically laid out by the common language
    //     runtime.
    AutoLayout = 0,
    //
    // Summary:
    //     Specifies that the type is a class.
    Class = 0,
    //
    // Summary:
    //     Specifies that the class is not public.
    NotPublic = 0,
    //
    // Summary:
    //     Specifies that the class is public.
    Public = 1,
    //
    // Summary:
    //     Specifies that the class is nested with public visibility.
    NestedPublic = 2,
    //
    // Summary:
    //     Specifies that the class is nested with private visibility.
    NestedPrivate = 3,
    //
    // Summary:
    //     Specifies that the class is nested with family visibility, and is thus accessible
    //     only by methods within its own type and any derived types.
    NestedFamily = 4,
    //
    // Summary:
    //     Specifies that the class is nested with assembly visibility, and is thus accessible
    //     only by methods within its assembly.
    NestedAssembly = 5,
    //
    // Summary:
    //     Specifies that the class is nested with assembly and family visibility, and is
    //     thus accessible only by methods lying in the intersection of its family and assembly.
    NestedFamANDAssem = 6,
    //
    // Summary:
    //     Specifies that the class is nested with family or assembly visibility, and is
    //     thus accessible only by methods lying in the union of its family and assembly.
    NestedFamORAssem = 7,
    //
    // Summary:
    //     Specifies type visibility information.
    VisibilityMask = 7,
    //
    // Summary:
    //     Specifies that class fields are laid out sequentially, in the order that the
    //     fields were emitted to the metadata.
    SequentialLayout = 8,
    //
    // Summary:
    //     Specifies that class fields are laid out at the specified offsets.
    ExplicitLayout = 16,
    //
    // Summary:
    //     Specifies class layout information.
    LayoutMask = 24,
    //
    // Summary:
    //     Specifies class semantics information; the current class is contextful (else
    //     agile).
    ClassSemanticsMask = 32,
    //
    // Summary:
    //     Specifies that the type is an interface.
    Interface = 32,
    //
    // Summary:
    //     Specifies that the type is abstract.
    Abstract = 128,
    //
    // Summary:
    //     Specifies that the class is concrete and cannot be extended.
    Sealed = 256,
    //
    // Summary:
    //     Specifies that the class is special in a way denoted by the name.
    SpecialName = 1024,
    //
    // Summary:
    //     Runtime should check name encoding.
    RTSpecialName = 2048,
    //
    // Summary:
    //     Specifies that the class or interface is imported from another module.
    Import = 4096,
    //
    // Summary:
    //     Specifies that the class can be serialized.
    Serializable = 8192,
    //
    // Summary:
    //     Specifies a Windows Runtime type.
    WindowsRuntime = 16384,
    //
    // Summary:
    //     LPTSTR is interpreted as UNICODE.
    UnicodeClass = 65536,
    //
    // Summary:
    //     LPTSTR is interpreted automatically.
    AutoClass = 131072,
    //
    // Summary:
    //     LPSTR is interpreted by some implementation-specific means, which includes the
    //     possibility of throwing a System.NotSupportedException. Not used in the Microsoft
    //     implementation of the .NET Framework.
    CustomFormatClass = 196608,
    //
    // Summary:
    //     Used to retrieve string information for native interoperability.
    StringFormatMask = 196608,
    //
    // Summary:
    //     Type has security associate with it.
    HasSecurity = 262144,
    //
    // Summary:
    //     Attributes reserved for runtime use.
    ReservedMask = 264192,
    //
    // Summary:
    //     Specifies that calling static methods of the type does not force the system to
    //     initialize the type.
    BeforeFieldInit = 1048576,
    //
    // Summary:
    //     Used to retrieve non-standard encoding information for native interop. The meaning
    //     of the values of these 2 bits is unspecified. Not used in the Microsoft implementation
    //     of the .NET Framework.
    CustomFormatMask = 12582912
};

namespace enum_ops
{
    template<typename T>
    concept operable_enum = std::is_enum_v<T> && requires(std::underlying_type_t<T> x) {
        { x | x } -> std::convertible_to<std::underlying_type_t<T>>;
        T(x);
    };

    template<operable_enum T>
    constexpr T operator|(T left, T right) noexcept
    {
        using U = std::underlying_type_t<T>;
        return T(U(left) | U(right));
    }

    template<operable_enum T>
    constexpr T operator&(T left, T right) noexcept
    {
        using U = std::underlying_type_t<T>;
        return T(U(left) & U(right));
    }

    template<operable_enum T>
    constexpr T& operator|=(T& left, T right) noexcept
    {
        return left = left | right;
    }

    template<operable_enum T>
    constexpr T& operator&=(T& left, T right) noexcept
    {
        return left = left & right;
    }

    template<operable_enum T>
    constexpr bool has_flag(T value, T flag) noexcept
    {
        return (value & flag) == flag;
    }
}

using enum_ops::operator|;
using enum_ops::operator&;
using enum_ops::operator|=;
using enum_ops::operator&=;
