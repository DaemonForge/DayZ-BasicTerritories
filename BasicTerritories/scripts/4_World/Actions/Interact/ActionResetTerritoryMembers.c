class ActionResetTerritoryMembers extends ActionInteractBase
{
	protected bool CanResetOwner = false;
	protected bool CanSetOwner = false;
	protected int NextSync = 0;
	
	override string GetText(){
		if (CanResetOwner) {
			if (CanSetOwner){
				return "Claim";
			}
			return "Reset Owner";
		}
		return "Clear All Members";
	}

	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ){	
		PlayerIdentity ident = PlayerIdentity.Cast(player.GetIdentity());
		TerritoryFlag theFlag = TerritoryFlag.Cast(target.GetObject());
		if (ident && theFlag){
			int curTime = GetGame().GetTime();
			if (!theFlag.CanReceiveNewOwner() && GetBasicTerritoriesConfig().ServerAdmins.Find(ident.GetId()) != -1){
				CanResetOwner = true;
				return true;
			}
			if (GetGame().IsClient() && theFlag.CanReceiveNewOwner() && NextSync <= curTime){
				NextSync = curTime + 80000;
				theFlag.SyncTerritory();
				return false;
			}
			float state = theFlag.GetAnimationPhase("flag_mast");
			if (theFlag.FindAttachmentBySlotName("Material_FPole_Flag") && state < TerritoryConst.FLAGUPSTATE){
				CanResetOwner = (theFlag.GetMemberCount() == 0);
				CanSetOwner = theFlag.CanReceiveNewOwner();
				bool CanResetMembers = !CanResetOwner && theFlag.CheckPlayerPermission(ident.GetId(), TerritoryPerm.REMOVEMEMBER);
				return (theFlag.CheckPlayerPermission(ident.GetId(), TerritoryPerm.ADMIN) || CanResetMembers || CanSetOwner);
			}
		}
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		if (action_data && action_data.m_Target && action_data.m_Player){
			PlayerBase thePlayer = PlayerBase.Cast(action_data.m_Player);
			TerritoryFlag theFlag = TerritoryFlag.Cast(action_data.m_Target.GetObject()	);
			if (theFlag && thePlayer && thePlayer.GetIdentity()){
				if (CanResetOwner){
					theFlag.SetTerritoryOwner("");
				}
				if (CanSetOwner && theFlag.CanReceiveNewOwner()){
					theFlag.SetTerritoryOwner(thePlayer.GetIdentity().GetId());
				}
				theFlag.ResetMembers();
			}
		}
	}
	
};