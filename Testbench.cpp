#include "Testbench.h"

#include "IPlug_include_in_plug_src.h"

Testbench::Testbench(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamVolume)->InitDouble("Volume", 0.5,0.0,1.0,0.1,"");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->EnableMouseOver(true);
    pGraphics->EnableMultiTouch(true);
    
#ifdef OS_WEB
    pGraphics->AttachPopupMenuControl();
#endif

//    pGraphics->EnableLiveEdit(true);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IText bigLabel {36, COLOR_WHITE, "Roboto-Regular", EAlign::Near, EVAlign::Top, 0};
    const IText smallLabel {18, COLOR_DARK_GRAY, "Roboto-Regular", EAlign::Near, EVAlign::Top, 0};
    
    const IRECT b = pGraphics->GetBounds().GetPadded(-10.f);

    const IRECT outputControls = b.GetGridCell(4,1,5);
    const int size = 50;

    const IVStyle YELLOW_STYLE = DEFAULT_STYLE.WithColor(kFG, COLOR_YELLOW);
    const IVStyle WHITE_STYLE = DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE);

    
    // outputControls

    pGraphics->AttachControl(new IVKnobControl(outputControls.GetGridCell(3,2,5).GetCentredInside(size), kParamVolume, "Volume",
                                               DEFAULT_STYLE.WithShowValue(false)));

  };
#endif
  
#if IPLUG_DSP
  dsp = new TestbenchDSP(&config);
#endif
}

#if IPLUG_DSP
void Testbench::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  //const double gain = GetParam(kGain)->Value() / 100.;

  // global
  config.volume = GetParam(kParamVolume)->Value();

  const int nChans = NOutChansConnected();
  config.samplerate = GetSampleRate();
  
  for (int s = 0; s < nFrames; s++) {
    const iplug::sample output = dsp->Tick();
    for (int c = 0; c < nChans; c++) {
      outputs[c][s] = output;
    }
  }
}

void Testbench::OnIdle()
{
}

void Testbench::OnReset()
{
}

void Testbench::ProcessMidiMsg(const IMidiMsg& msg)
{
  TRACE;
  
  int status = msg.StatusMsg();
  
  switch (status)
  {
    case IMidiMsg::kNoteOn:
    case IMidiMsg::kNoteOff:
    case IMidiMsg::kPolyAftertouch:
    case IMidiMsg::kControlChange:
    case IMidiMsg::kProgramChange:
    case IMidiMsg::kChannelAftertouch:
    case IMidiMsg::kPitchWheel:
    {
      goto handle;
    }
    default:
      return;
  }
  
handle:
  if(status == IMidiMsg::kNoteOn) {
    dsp->NoteOn(msg.NoteNumber(), msg.Velocity());
  }
  if (status == IMidiMsg::kNoteOff) {
    dsp->NoteOff(msg.NoteNumber());
  }
  if (status == IMidiMsg::kControlChange) {
    if (msg.ControlChangeIdx() == IMidiMsg::kModWheel) {
      dsp->ModWheel(msg.ControlChange(IMidiMsg::kModWheel));
    }
  }
  if (status == IMidiMsg::kPitchWheel) {
    dsp->BendWheel(msg.PitchWheel());
  }
  // mDSP.ProcessMidiMsg(msg);
  SendMidiMsg(msg);
}

void Testbench::OnParamChange(int paramIdx)
{
  // mDSP.SetParam(paramIdx, GetParam(paramIdx)->Value());
}

bool Testbench::OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData)
{
  if(ctrlTag == kCtrlTagBender && msgTag == IWheelControl::kMessageTagSetPitchBendRange)
  {
    const int bendRange = *static_cast<const int*>(pData);
    // mDSP.mSynth.SetPitchBendRange(bendRange);
  }
  
  return false;
}
#endif
