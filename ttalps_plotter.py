from ROOT import TFile, TGraph, TCanvas, gPad, gStyle, gROOT, TLegend, TLine, TColor
import ROOT
import importlib
import sys, os
import os.path
import ntpath

def setupLegend(legend):
  legend.SetBorderSize(0)
  # legend.SetFillColor(0)
  # legend.SetFillStyle(0)
  legend.SetTextFont(42)
  legend.SetTextSize(0.035)

def getStackandLegendDicts(config,file_type,efficiency=False):
  hists_dict = {}
  legends_dict = {}
  legend_position = config.signal_legend_position
  if(file_type=="background"):
    legend_position = config.background_legend_position

  if(efficiency == False):
    for variable, params in config.variables.items():
      hists_dict[variable] = ROOT.THStack(variable+file_type,variable+file_type)

      legend = TLegend(legend_position[0],legend_position[1],legend_position[2],legend_position[3])
      setupLegend(legend)
      legends_dict[variable] = legend
  else:
    for efficiency, params in config.efficiency_plots.items():
      variable, title, xlabel, ylabel = params
      hists_dict[efficiency] = ROOT.THStack(variable+file_type+"_eff",variable+file_type+"_eff")

      legend = TLegend(legend_position[0],legend_position[1],legend_position[2],legend_position[3])
      setupLegend(legend)
      legends_dict[efficiency] = legend

  return hists_dict, legends_dict

def setupHist(hist,config,params,linestyles,file_type):
  title,norm1,rebin,xmin,xmax,ymin,ymax,xlabel,ylabel = params
  line_color, line_style = linestyles
  if(file_type=="signal"):
    hist.SetLineStyle(line_style)
    hist.SetLineColor(line_color)
  else:
    hist.SetLineColorAlpha(line_color, 0)
    hist.SetFillColorAlpha(line_color, 0.7)
  hist.Rebin(rebin)
  if(norm1):
    hist.Scale(1./hist.Integral())
  hist.Sumw2(False)

def setupFigure(stack, params):
  title,norm1,rebin,xmin,xmax,ymin,ymax,xlabel,ylabel = params

  if(ymin>0):
    stack.GetYaxis().SetMinimum(ymin)
  if(ymax>0):
    stack.GetYaxis().SetMaximum(ymax)

  stack.GetXaxis().SetLimits(xmin, xmax)
  stack.GetXaxis().SetTitle(xlabel)
  stack.GetYaxis().SetTitle(ylabel)
  stack.SetTitle(title)


def getEfficiencyHist(input_hist):
  hist = input_hist.Clone()
  initial = hist.GetBinContent(1)
  print(initial)
  for bin in range(1, hist.GetNbinsX()+2):
    print(bin,"  ",hist.GetBinContent(bin), "  ", hist.GetBinContent(bin)/initial)
    hist.SetBinContent(bin, hist.GetBinContent(bin)/initial)
  return hist


def addHistsToStacks(config, input_files, filename, signal_hists, background_hists, signal_legends, background_legends, file_type, efficiency=False):
  backgrounds_included = False
  if(efficiency==False):
    for variable, params in config.variables.items():
      hist = input_files[filename].Get(variable)
      if(hist.GetEntries() == 0):
        continue
      setupHist(hist, config, params, config.lines[filename], file_type)
      
      if(file_type=="signal"):
        signal_hists[variable].Add(hist)
        signal_legends[variable].AddEntry(hist, config.legends[filename], "l")
      else:
        background_hists[variable].Add(hist)
        background_legends[variable].AddEntry(hist, config.legends[filename], "f")
        backgrounds_included = True
  else:
    for efficiency, eff_params in config.efficiency_plots.items():
      variable, title, xlabel, ylabel = eff_params
      params = config.variables[variable]
      hist = input_files[filename].Get(variable)
      if(hist.GetEntries() == 0):
        continue
      setupHist(hist, config, params, config.lines[filename], file_type)
      
      hist_eff = getEfficiencyHist(hist)
      
      if(file_type=="signal"):
        signal_hists[efficiency].Add(hist_eff)
        signal_legends[efficiency].AddEntry(hist_eff, config.legends[filename], "l")
      else:
        background_hists[efficiency].Add(hist_eff)
        background_legends[efficiency].AddEntry(hist_eff, config.legends[filename], "f")
        backgrounds_included = True

  return backgrounds_included


def drawStacks(config, backgrounds_included, signal_hists, background_hists, signal_legends, background_legends, efficiency=False):
  if(efficiency==False):
    for variable, params in config.variables.items():
      canvas = TCanvas("canvas_"+variable, "canvas_"+variable, 800, 600)
      canvas.cd()

      canvas.SetLogy()

      #staring with background stack
      if(backgrounds_included):
        background_hists[variable].Draw("hist nostack")
        setupFigure(background_hists[variable], params)

        signal_hists[variable].Draw("nostack same")
      else:
        signal_hists[variable].Draw("hist nostack")
        setupFigure(signal_hists[variable], params)

      signal_legends[variable].Draw()
      background_legends[variable].Draw()
    
      canvas.Update()
      canvas.SaveAs(config.output_path+"/"+variable+".pdf")
  else:
    for efficiency, eff_params in config.efficiency_plots.items():
      variable, title, xlabel, ylabel = eff_params
      params = config.variables[variable]

      canvas = TCanvas("canvas_"+variable+"_eff", "canvas_"+variable+"_eff", 800, 600)
      canvas.cd()

      canvas.SetLogy()

      #staring with background stack
      if(backgrounds_included):
        background_hists[efficiency].Draw("hist nostack")
        setupFigure(background_hists[efficiency], params)

        signal_hists[efficiency].Draw("nostack same")
      else:
        signal_hists[efficiency].Draw("hist nostack")
        setupFigure(signal_hists[efficiency], params)

      signal_legends[efficiency].Draw()
      background_legends[efficiency].Draw()
    
      canvas.Update()
      canvas.SaveAs(config.output_path+"/"+efficiency+".pdf")


def main():

  configPath = sys.argv[1]
  if(".py" in configPath):
    configPath = configPath[:-3]
  config = importlib.import_module(configPath)

  signal_hists, signal_legends = getStackandLegendDicts(config,"signal")
  signal_eff_hists, signal_eff_legends = getStackandLegendDicts(config,"signal", True)
  background_hists, background_legends = getStackandLegendDicts(config,"background")
  background_eff_hists, background_eff_legends = getStackandLegendDicts(config,"background", True)

  backgrounds_included = False
  input_files = {}

  for name, fileInfo in config.files.items():
    filename, file_type = fileInfo
    if(file_type=="signal"):
      input_path = config.signal_path
    else:
      input_path = config.background_path
    input_files[name] = TFile.Open(input_path+"/"+filename, "READ")

    backgrounds_included = addHistsToStacks(config, input_files, name, signal_hists, background_hists, signal_legends, background_legends, file_type)
    backgrounds_included = addHistsToStacks(config, input_files, name, signal_eff_hists, background_eff_hists, signal_eff_legends, background_eff_legends, file_type, True)
      
  drawStacks(config, backgrounds_included, signal_hists, background_hists, signal_legends, background_legends)
  drawStacks(config, backgrounds_included, signal_eff_hists, background_eff_hists, signal_eff_legends, background_eff_legends, True)
  

if __name__ == "__main__":
    main()