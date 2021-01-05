modded class UAMisc
{
	const float FLAG_STEP_INCREMENT = 0.025; //0.2
	const float BONUS_FLAG_STEP_INCREMENT = 0.175; //0.2
	
};

modded class ActionRaiseFlag: ActionContinuousBase
{
	override void OnFinishProgressServer( ActionData action_data )
	{
		super.OnFinishProgressServer( action_data );
		TerritoryFlag totem = TerritoryFlag.Cast( action_data.m_Target.GetObject() );
		if ( totem && UAMisc.BONUS_FLAG_STEP_INCREMENT > 0)
		{
			totem.AnimateFlag(totem.GetAnimationPhase("flag_mast") - UAMisc.BONUS_FLAG_STEP_INCREMENT);
			totem.AddRefresherTime01(UAMisc.BONUS_FLAG_STEP_INCREMENT);
		}
	}
};