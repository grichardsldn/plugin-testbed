#pragma once

#include "config.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"
#include "TestbenchDSP.h"
#include "TestbenchConfig.h"

const int kNumPresets = 1;

enum EParams
{
  kParamTune = 0,
  kParamVolume, 
  kParamLFORate, 
  kParamA,
  kParamB,
  kParamC,
  kParamD,
  kNumParams
};

#if IPLUG_DSP

#endif

enum EControlTags
{
  kCtrlTagMeter = 0,
  kCtrlTagLFOVis,
  kCtrlTagScope,
  kCtrlTagRTText,
  kCtrlTagKeyboard,
  kCtrlTagBender,
  kNumCtrlTags
};

using namespace iplug;
using namespace igraphics;

class Testbench final : public Plugin
{
public:
  Testbench(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
public:
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;
  bool OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData) override;

private:
  TestbenchDSP* dsp;
  TestbenchConfig config;
#endif
};
