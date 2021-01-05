class ActionAddTerritoryMember extends ActionInteractBase
{
	
	protected bool AddingMember = true;

	void ActionAddTerritoryMember(){
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}
	
	override string GetText(){
		if (AddingMember){
			return "Add Member";
		} 
		
		return "Cancel Adding Member";
	}

	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ){	
		PlayerIdentity ident = PlayerIdentity.Cast(player.GetIdentity());
		TerritoryFlag theFlag = TerritoryFlag.Cast(target.GetObject());
		if (ident && theFlag){
			float state = theFlag.GetAnimationPhase("flag_mast");
			if (theFlag.FindAttachmentBySlotName("Material_FPole_Flag") && state < TerritoryConst.FLAGUPSTATE){
				AddingMember = !theFlag.CanAddMember();
				return theFlag.CheckPlayerPermission(ident.GetId(), TerritoryPerm.ADDMEMBER);
			}
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		if (action_data && action_data.m_Target){
			TerritoryFlag theFlag = TerritoryFlag.Cast(action_data.m_Target.GetObject()	);
			if (theFlag){
				theFlag.AllowMemberToBeAdded(AddingMember);
			}
		}
	}
	
};