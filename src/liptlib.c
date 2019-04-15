/**
  * @file
  * @author  azenk
  * @version
  * @date
  * @brief
  */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define liptlib_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "comm_socket.h"
#if defined ( __linux )
#include <unistd.h>
#else
#include <windows.h>
#endif


//定义宏用来方便使用设置键值
#define LUA_ENUM(L, val) \
  lua_pushliteral(L, #val); \
  lua_pushnumber(L, val); \
  lua_settable(L, -3)

enum __KeyID
{
	N0 = 0,
	N1,
	N2,
	N3,
	N4,
	N5,
	N6,
	N7,
	N8,
	N9,

	BACK = 0x0A,
	ENTER = 0x0B,
	PROG = 0x0C,

	UP = 0x10,
	DOWN = 0x20,
};

enum __KeyOption
{
	NONE = 0,
	PRESSED,
	RELEASED,
	LONG_PRESSED,
	LONG_RELEASED,
};

enum __Switch
{
	OPEN = 0,
	CLOSE,
	DONTCARE,
	UNKNOWN,
};

enum __BatteryID
{
	RTC = 0,
	BACKUP,
};

enum __Battery
{
	FULL = 0,
	LOW,
	EMPTY,
};

enum __SensorID
{
    MAIN_COVER = 0,
    SUB_COVER,
    MAGNETIC,
};

struct __key_data
{
    uint16_t ID;
    enum __KeyOption Status;
};

struct __battery_data
{
    enum __BatteryID Type;
    enum __Battery Status;
};

struct __sensors_data
{
    enum __SensorID Type;
    enum __Switch Status;
};

struct __relay_data
{
    enum __Switch Status;
};

static struct __key_data KeyData;
static struct __battery_data BatteryData;
static struct __sensors_data SensorsData;
static struct __relay_data RelayData;

static SOCKET sock_key = INVALID_SOCKET;
static SOCKADDR_IN src_key;
static SOCKET sock_battery = INVALID_SOCKET;
static SOCKADDR_IN src_battery;
static SOCKET sock_sensors = INVALID_SOCKET;
static SOCKADDR_IN src_sensors;
static SOCKET sock_relay = INVALID_SOCKET;
static SOCKADDR_IN src_relay;


static int ipt_keyboard(lua_State *L)
{
	int n = lua_gettop(L);
	int param[4];
	int i;
	
	memset(&KeyData, 0, sizeof(KeyData));
	
	if(n > 4)
	{
		lua_pushliteral(L, "incorrect argument");
		lua_error(L);
	}
	
	for (i = 1; i <= n; i++)
	{
		if (!lua_isnumber(L, i))
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
		
		lua_number2integer(param[i-1], lua_tonumber(L, i));
	}
	
	switch((enum __KeyID)param[0])
	{
		case N0:
		case N1:
		case N2:
		case N3:
		case N4:
		case N5:
		case N6:
		case N7:
		case N8:
		case N9:
		case BACK:
		case ENTER:
		case PROG:
		case UP:
		case DOWN:
		{
			KeyData.ID = (uint16_t)param[0];
			break;
		}
		default:
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
	}
	
	switch((enum __KeyOption)param[1])
	{
		case PRESSED:
		case RELEASED:
		case LONG_PRESSED:
		case LONG_RELEASED:
		{
			KeyData.Status = (enum __KeyOption)param[1];
			break;
		}
		case NONE:
		{
			return 1;
		}
		default:
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
	}
	
	sock_key = emitter.open(50006, &src_key);

	if(sock_key != INVALID_SOCKET)
	{
		emitter.write(sock_key, &src_key, (uint8_t *)&KeyData, sizeof(KeyData));
#if defined ( __linux )
		usleep(200*1000);
#else
		Sleep(200);
#endif
		emitter.close(sock_key);
        sock_key = INVALID_SOCKET;
	}
	
	return 1;
}

static int ipt_relay(lua_State *L)
{
	int n = lua_gettop(L);
	int param[4];
	int i;
	
	memset(&RelayData, 0, sizeof(RelayData));
	
	if(n > 4)
	{
		lua_pushliteral(L, "incorrect argument");
		lua_error(L);
	}
	
	for (i = 1; i <= n; i++)
	{
		if (!lua_isnumber(L, i))
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
		
		lua_number2integer(param[i-1], lua_tonumber(L, i));
	}
	
	switch((enum __Switch)param[0])
	{
		case OPEN:
		case CLOSE:
		case DONTCARE:
		case UNKNOWN:
		{
			RelayData.Status = (enum __Switch)param[0];
			break;
		}
		default:
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
	}
	
	sock_relay = emitter.open(50004, &src_relay);

	if(sock_relay != INVALID_SOCKET)
	{
		emitter.write(sock_relay, &src_relay, (uint8_t *)&RelayData, sizeof(RelayData));
#if defined ( __linux )
		usleep(200*1000);
#else
		Sleep(200);
#endif
		emitter.close(sock_relay);
        sock_relay = INVALID_SOCKET;
	}
	
	return 1;
}

static int ipt_battery(lua_State *L)
{
	int n = lua_gettop(L);
	int param[4];
	int i;
	
	memset(&BatteryData, 0, sizeof(BatteryData));
	
	if(n > 4)
	{
		lua_pushliteral(L, "incorrect argument");
		lua_error(L);
	}
	
	for (i = 1; i <= n; i++)
	{
		if (!lua_isnumber(L, i))
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
		
		lua_number2integer(param[i-1], lua_tonumber(L, i));
	}
	
	switch((enum __BatteryID)param[0])
	{
		case RTC:
		case BACKUP:
		{
			BatteryData.Type = (enum __BatteryID)param[0];
			break;
		}
		default:
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
	}

	switch((enum __Battery)param[1])
	{
		case FULL:
		case LOW:
		case EMPTY:
		{
			BatteryData.Status = (enum __Battery)param[1];
			break;
		}
		default:
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
	}
	
	sock_battery = emitter.open(50005, &src_battery);

	if(sock_battery != INVALID_SOCKET)
	{
		emitter.write(sock_battery, &src_battery, (uint8_t *)&BatteryData, sizeof(BatteryData));
#if defined ( __linux )
		usleep(200*1000);
#else
		Sleep(200);
#endif
		emitter.close(sock_battery);
        sock_battery = INVALID_SOCKET;
	}
	
	return 1;
}

static int ipt_sensors(lua_State *L)
{
	int n = lua_gettop(L);
	int param[4];
	int i;
	
	memset(&SensorsData, 0, sizeof(SensorsData));
	
	if(n > 4)
	{
		lua_pushliteral(L, "incorrect argument");
		lua_error(L);
	}
	
	for (i = 1; i <= n; i++)
	{
		if (!lua_isnumber(L, i))
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
		
		lua_number2integer(param[i-1], lua_tonumber(L, i));
	}
	
	switch((enum __SensorID)param[0])
	{
		case MAIN_COVER:
		case SUB_COVER:
		case MAGNETIC:
		{
			SensorsData.Type = (enum __SensorID)param[0];
			break;
		}
		default:
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
	}
	
	switch((enum __Switch)param[1])
	{
		case OPEN:
		case CLOSE:
		case DONTCARE:
		case UNKNOWN:
		{
			SensorsData.Status = (enum __Switch)param[1];
			break;
		}
		default:
		{
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}
	}
	
	sock_sensors = emitter.open(50003, &src_sensors);

	if(sock_sensors != INVALID_SOCKET)
	{
		emitter.write(sock_sensors, &src_sensors, (uint8_t *)&BatteryData, sizeof(BatteryData));
#if defined ( __linux )
		usleep(200*1000);
#else
		Sleep(200);
#endif
		emitter.close(sock_sensors);
        sock_sensors = INVALID_SOCKET;
	}
	
	return 1;
}



static const luaL_Reg syslib[] =
{
	{"keyboard", ipt_keyboard},
	{"relay", ipt_relay},
	{"battery", ipt_battery},
	{"sensors", ipt_sensors},
	{NULL, NULL}
};

/* }====================================================== */



LUALIB_API int luaopen_input(lua_State *L)
{
	luaL_register(L, "input", syslib);
	
	//注册__KeyID枚举
	lua_newtable(L);
	LUA_ENUM(L, N0);
	LUA_ENUM(L, N1);
	LUA_ENUM(L, N2);
	LUA_ENUM(L, N3);
	LUA_ENUM(L, N4);
	LUA_ENUM(L, N5);
	LUA_ENUM(L, N6);
	LUA_ENUM(L, N7);
	LUA_ENUM(L, N8);
	LUA_ENUM(L, N9);
	LUA_ENUM(L, BACK);
	LUA_ENUM(L, ENTER);
	LUA_ENUM(L, PROG);
	LUA_ENUM(L, UP);
	LUA_ENUM(L, DOWN);
	lua_setglobal(L,"__KeyID");
	
	//注册__KeyOption枚举
	lua_newtable(L);
	LUA_ENUM(L, NONE);
	LUA_ENUM(L, PRESSED);
	LUA_ENUM(L, RELEASED);
	LUA_ENUM(L, LONG_PRESSED);
	LUA_ENUM(L, LONG_RELEASED);
	lua_setglobal(L,"__KeyOption");
	
	//注册__Switch枚举
	lua_newtable(L);
	LUA_ENUM(L, OPEN);
	LUA_ENUM(L, CLOSE);
	LUA_ENUM(L, DONTCARE);
	LUA_ENUM(L, UNKNOWN);
	lua_setglobal(L,"__Switch");
	
	//注册__BatteryID枚举
	lua_newtable(L);
	LUA_ENUM(L, RTC);
	LUA_ENUM(L, BACKUP);
	lua_setglobal(L,"__BatteryID");
	
	//注册__Battery枚举
	lua_newtable(L);
	LUA_ENUM(L, FULL);
	LUA_ENUM(L, LOW);
	LUA_ENUM(L, EMPTY);
	lua_setglobal(L,"__Battery");
	
	//注册__SensorID枚举
	lua_newtable(L);
	LUA_ENUM(L, MAIN_COVER);
	LUA_ENUM(L, SUB_COVER);
	LUA_ENUM(L, MAGNETIC);
	lua_setglobal(L,"__SensorID");

	
	memset(&KeyData, 0, sizeof(KeyData));
	memset(&BatteryData, 0, sizeof(BatteryData));
	memset(&SensorsData, 0, sizeof(SensorsData));
	memset(&RelayData, 0, sizeof(RelayData));
	
	return 1;
}

