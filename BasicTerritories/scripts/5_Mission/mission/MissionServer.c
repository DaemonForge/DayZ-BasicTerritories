modded class MissionServer extends MissionBase
{
	override void OnInit() {
		super.OnInit();
		
		Print("[BasicTerritories] OnInit");
		GetBasicTerritoriesConfig();
		GetRPCManager().AddRPC( "BASICT", "RPCBasicTerritoriesModSettings", this, SingeplayerExecutionType.Both );
	}
	
	void RPCBasicTerritoriesModSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		PlayerIdentity RequestedBy = PlayerIdentity.Cast(sender);
		if (RequestedBy){
			GetRPCManager().SendRPC("BASICT", "RPCBasicTerritoriesModSettings", new Param1< BasicTerritoriesConfig >( GetBasicTerritoriesConfig() ), true, RequestedBy);
		}
	}
}


