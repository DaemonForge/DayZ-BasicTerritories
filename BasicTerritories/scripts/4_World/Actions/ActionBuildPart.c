modded class ActionBuildPart: ActionContinuousBase
{
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		
		
		TerritoryFlag theFlag = TerritoryFlag.Cast( action_data.m_Target.GetObject() );
		if (!theFlag)
			return;
			
		
		Construction construction = theFlag.GetConstruction();
		
		string part_name = BuildPartActionData.Cast(action_data).m_PartType;
		
		if ( construction && construction.IsPartConstructed(part_name) && part_name == "base" ) {
			if (action_data.m_MainItem) {
				action_data.m_MainItem.DecreaseHealth( GetBasicTerritoriesConfig().BuildBonusSledgeDamage, false );
			}
		}
	}
	
}