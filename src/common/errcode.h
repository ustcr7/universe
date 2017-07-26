#ifndef __UNIVERSE_BASE_COMMON_ERRCODE_H_
#define __UNIVERSE_BASE_COMMON_ERRCODE_H_


enum UNIVERSE_ERRCODE{
	ERR_INVALID_PARAM                 = -10001,
	ERR_NO_DATA                       = -10002,
	ERR_UNKNOWN                       = -10003,
	ERR_NOT_FOUND                     = -10004,
	ERR_EXISTED                       = -10005,
	ERR_NOT_EXISTS                    = -10006,
	ERR_BAD_ALLOC                     = -10007,
	ERR_NOT_ENOUGH                    = -10008,
	ERR_ALREADY_EXISTS                = -10009,


	//-----spell error code------
	ERR_SPELL_NOT_LEARNED             = -20001,
	ERR_SPELL_IN_CD                   = -20002,
	ERR_SPELL_TOO_FAR_AWAY            = -20003, //距离过远无法施法
	ERR_SPELL_MP_NOT_ENOUGH           = -20004, //魔法值不够
	ERR_SPELL_ALREADY_LEARNED         = -20005, //该技能已经学会

	//-----instance error code -------
	ERR_INST_NOT_IN_INSTANCE          = -30001,
	ERR_INST_NOT_IN_SAME_INSTANCEE    = -30002,

	//------quest error code --------
	ERR_QUEST_NOT_ACCEPT              = -40001,

};



#endif
