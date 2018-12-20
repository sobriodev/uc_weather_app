/*
 * commands.c
 *
 *  Created on: 11 gru 2018
 *      Author: sobriodev
 */

#include "commands.h"
#include "app_logic.h"
#include "board.h"

/* ------------------------------------------------------------------------------------------- */
/* ----------------------------- Private functions and variables ----------------------------- */
/* ------------------------------------------------------------------------------------------- */

/*!
 * @brief Array of available commands
 */
const command commands[COMMANDS_COUNT] =
{
		{ "[D1]", bme280_set_delay_100_ms },
		{ "[D2]", bme280_set_delay_1_s },
		{ "[D3]", bme280_set_delay_10_s }
};

/*!
 * @brief Check if input command equals pattern command
 * @param[in] cmd : Pointer to pattern command
 * @param[in] input_cmd : Input command
 * @return True if commands are equal, false otherwise
 */
bool command_eq(const command *cmd, const char *input_cmd)
{
	for (int32_t i = 0; i < COMMMAND_LEN; i++) {
		if (cmd->name[i] != input_cmd[i]) {
			return false;
		}
	}
	return true;
}

/* ------------------------------------------------------------------------------------------- */
/* -------------------------------------- API functions -------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

bool invoke_cmd(const char *cmd)
{
	for (int i = 0; i < COMMANDS_COUNT; i++) {
		if (command_eq(&commands[i], cmd)) {
			commands[i].callback();
			Board_LED_Toggle(0);
			return true;
		}
	}
	return false;
}
