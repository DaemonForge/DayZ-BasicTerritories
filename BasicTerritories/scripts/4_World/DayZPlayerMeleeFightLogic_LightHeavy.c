modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	
	override protected void EvaluateHit(InventoryItem weapon)
	{
		super.EvaluateHit( weapon );
		EntityAI target = EntityAI.Cast(m_MeleeCombat.GetTargetEntity());
		if( target && weapon ){
			Print("[Territories] [Debug] EvaluateHit Weapon: " + weapon.GetType() + "( HP:" + weapon.GetHealth("","") + ") Target: " + target.GetType() + "( HP:" + target.GetHealth("","") + ")");
		} else if (weapon){
			Print("[Territories] [Debug] EvaluateHit Weapon: " + weapon.GetType()  + "( HP:" + weapon.GetHealth("","") + ") Target: NULL ");
		} else if (target){
			Print("[Territories] [Debug] EvaluateHit Weapon: NULL Target: " + target.GetType()  + "( HP:" + target.GetHealth("","") + ")");
		} else {
			Print("[Territories] [Debug] EvaluateHit Weapon: NULL Target: NULL");
		}
	}
	
	
}