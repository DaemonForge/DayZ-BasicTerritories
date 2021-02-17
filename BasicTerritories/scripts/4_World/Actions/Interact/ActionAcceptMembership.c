class ActionAcceptMembership extends ActionInteractBase
{
	
	bool IsMember = false;

	void ActionAcceptMembership(){
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override string GetText(){
		if (IsMember){
			return "Leave Territory";
		}
		return "Accept Membership";
	}

	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ){	
		TerritoryFlag theFlag = TerritoryFlag.Cast(target.GetObject());
		PlayerIdentity ident = PlayerIdentity.Cast(player.GetIdentity());
		if (ident && theFlag){
			float state = theFlag.GetAnimationPhase("flag_mast");
			if (theFlag.FindAttachmentBySlotName("Material_FPole_Flag") && state < TerritoryConst.FLAGUPSTATE){
				IsMember = (theFlag.IsTerritoryMember(ident.GetId()) && !theFlag.CanReceiveNewOwner());
				if (theFlag.CanAddMember() && !IsMember){
					return true;
				} else if (IsMember && !theFlag.IsTerritoryOwner(ident.GetId())){
					return true;
				}
			}
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		if (action_data && action_data.m_Target && action_data.m_Player){
			Print("ActionAcceptMembership - OnExecuteServer");
			TerritoryFlag theFlag = TerritoryFlag.Cast(action_data.m_Target.GetObject());
			PlayerBase thePlayer = PlayerBase.Cast(action_data.m_Player);
			if (theFlag && thePlayer && thePlayer.GetIdentity()){
				Print("ActionAcceptMembership - OnExecuteServer - AddMember - " + thePlayer.GetIdentity().GetId());
				if (!theFlag.IsTerritoryMember(thePlayer.GetIdentity().GetId())){
					theFlag.AddMember(thePlayer.GetIdentity().GetId());
				} else {
					theFlag.RemoveMember(thePlayer.GetIdentity().GetId());
				}
			}
		}
	}
};