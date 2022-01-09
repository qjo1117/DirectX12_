#pragma once

#define DEVICE				GEngine->GetDevice()->GetDevice()
#define CMD_LIST			GEngine->GetCmdQueue()->GetCmdList()
#define ROOT_SIGNATURE		GEngine->GetRootSignature()->GetSignature()
#define RESOURCE_CMD_LIST	GEngine->GetCmdQueue()->GetResourceCmdList()
#define CONST_BUFFER(type)	GEngine->GetConstantBuffer(type)

<<<<<<< Updated upstream
#define INPUT				Input::GetI()
#define TIMER				Timer::GetI()
=======
#define DELTATIME			GET_SINGLE(Timer)->GetDeltaTime()
#define INPUT				GET_SINGLE(Input)
#define GUI					GEngine->GetGUI()
>>>>>>> Stashed changes

#define DELTATIME			Timer::GetI()->GetDeltaTime()