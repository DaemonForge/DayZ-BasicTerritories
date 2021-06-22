class CfgPatches
{
	class MSPDismantleFix
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={
			"MSP_Scripts",
			"BasicTerritories"
		};
	};
};
class CfgMods
{
	class MSPDismantleFix
	{
		dir="MSPDismantleFix";
        name="MSPDismantleFix";
        credits="DaemonForge";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={ "World" };
	    class defs
	    {
			class worldScriptModule
            {
                value="";
                files[]={ "MSPDismantleFix/scripts/4_World" };
            };
			
        };
    };
};
