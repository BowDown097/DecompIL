namespace DotnetBinaryInterface.MarshalTypes;

[StructLayout(LayoutKind.Sequential)]
public readonly struct MarshalException
{
    public readonly MarshalString className;
    public readonly MarshalString message;
    public readonly MarshalString stackTrace;
    public readonly MarshalNullable<MarshalException> innerException;

    public MarshalException(Exception? managedException)
    {
        if (managedException is not null)
        {
            className = new MarshalString(managedException.GetType().ToString());
            message = new MarshalString(managedException.Message);
            stackTrace = new MarshalString(managedException.StackTrace);
            if (managedException.InnerException is not null)
                innerException = new MarshalException(managedException.InnerException);
        }
    }
}