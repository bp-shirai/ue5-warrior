// Fill out your copyright notice in the Description page of Project Settings.

#include "WTypes/WCountdownAction.h"



void FWCountdownAction::UpdateOperation(FLatentResponse& Response)
{
    if (bNeedToCancel)
    {
        CountdownOutput = EWCountdownActionOutput::Canceled;

        Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
        return;
    }

    if (ElapsedTimeSinceStart >= TotalCountdownTime)
    {
        CountdownOutput = EWCountdownActionOutput::Completed;

        Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
        return;
    }

    if (ElapsedInterval < UpdateInterval)
    {
        ElapsedInterval += Response.ElapsedTime();
    }
    else
    {
        ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();
        //ElapsedTimeSinceStart += UpdateInterval > 0.f ? ElapsedInterval : Response.ElapsedTime();

        OutRemainingTime = TotalCountdownTime - ElapsedTimeSinceStart;

        CountdownOutput = EWCountdownActionOutput::Updated;

        Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

        ElapsedInterval = 0.f;
    }

}

void FWCountdownAction::CancelAction()
{
	bNeedToCancel = true;
}
