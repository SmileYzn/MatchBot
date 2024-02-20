# HLDS Path
$hldsPath = "D:\Files\Desktop\hlds";

# Executable path
$hldsExecutable = "D:\Files\Desktop\hlds\hlds.exe";

# Get Ethernet address as IPV4
$hldsIP = (Get-NetIPAddress -AddressFamily IPV4 -InterfaceAlias Ethernet).IPAddress;

# Executable parameters
$hldsParam = "-console -game cstrike -port 27020 -pingboost 3 -master -secure -bots +ip $($hldsIP) +map de_inferno +maxplayers 32 +sys_ticrate 1000";

# DLL Path
$dllPath = "$($hldsPath)\cstrike\addons\matchbot\dlls";

# Release DLL
$releaseDLL = (Resolve-Path("$($PSScriptRoot)\..\Release\matchbot_mm.dll")).Path;

# Find HLDS Process
$hlds = Get-Process -Name hlds -ErrorAction SilentlyContinue;

# If found
if ($hlds)
{
    # If is started 
    if (!$hlds.started)
    {
        # Get HLDS path
        $hldsPath = (Get-Item $hlds.Path).DirectoryName;

        # Set HLDS executable path
        $hldsExecutable = $hlds.Path;
        
        # Set DLL path
        $dllPath = "$($hldsPath)\cstrike\addons\matchbot\dlls";

        # Force kill
        $hlds | Stop-Process -Force -ErrorAction SilentlyContinue
    }
}

# Wait 1 second
sleep(1);

# Check if path exists
if(Test-Path($dllPath))
{
    # Copy DLL to path
    Copy-Item  -Path $releaseDLL -Destination $dllPath -Recurse -Force

    # Start HLDS
    Start-Process -FilePath $hldsExecutable -ArgumentList $hldsParam -WorkingDirectory $hldsPath -WindowStyle Minimized
}
else
{
    # Failed to run
    Write-Host("Cannot copy DLL and run HLDS: Path not found $($dllPath)");
}
