# BESS System Calculator & Visualizer

<div style="max-width:750px;padding:1rem;border:1px solid #ccc;border-radius:8px;">

<h3>Cell Selection & Specs</h3>
<label>Cell Type:</label>
<select id="cell_type" onchange="setCellValues()" style="width:100%;margin-bottom:10px;">
  <option value="">-- Select --</option>
  <option value="NMC">NMC (3.7V, 5Ah)</option>
  <option value="LFP">LFP (3.2V, 100Ah)</option>
  <option value="CUSTOM">Custom</option>
</select>

<label>Cell Voltage (V):</label>
<input id="cell_voltage" type="number" style="width:100%;margin-bottom:10px;">

<label>Cell Capacity (Ah):</label>
<input id="cell_capacity" type="number" style="width:100%;margin-bottom:10px;">

<label>Cell Cost (USD per cell):</label>
<input id="cell_cost" type="number" value="5" style="width:100%;margin-bottom:10px;">

<label>SoC Start (%):</label>
<input id="bess_soc_start" type="number" value="100" style="width:100%;margin-bottom:10px;">

<label>SoC End (%):</label>
<input id="bess_soc_end" type="number" value="20" style="width:100%;margin-bottom:10px;">

<label>Efficiency (%):</label>
<input id="bess_eff" type="number" value="95" style="width:100%;margin-bottom:10px;">

<label>Required System Size (kWh):</label>
<input id="system_kwh" type="number" style="width:100%;margin-bottom:10px;">

<hr/>

<h3>Module Configuration</h3>
<label>Cells in Series per Module:</label>
<input id="series_cells" type="number" value="16" style="width:100%;margin-bottom:10px;">

<label>Cells in Parallel per Module:</label>
<input id="parallel_cells" type="number" value="1" style="width:100%;margin-bottom:10px;">

<label>Modules per Pack:</label>
<input id="modules_per_pack" type="number" value="10" style="width:100%;margin-bottom:10px;">

<hr/>

<h3>Pack & System Configuration</h3>
<label>Packs in Series:</label>
<input id="packs_series" type="number" value="2" style="width:100%;margin-bottom:10px;">

<label>Packs in Parallel:</label>
<input id="packs_parallel" type="number" value="2" style="width:100%;margin-bottom:10px;">

<hr/>

<h3>Cost Parameters</h3>
<label>Extra Electronics Cost per Module (USD):</label>
<input id="module_extra_cost" type="number" value="50" style="width:100%;margin-bottom:10px;">

<label>Extra Electronics Cost per Pack (USD):</label>
<input id="pack_extra_cost" type="number" value="100" style="width:100%;margin-bottom:10px;">

<label>Container Cost (USD per System):</label>
<input id="container_cost" type="number" value="500" style="width:100%;margin-bottom:10px;">

<button onclick="calcBess()" style="margin-top:10px;">Calculate & Draw</button>

<hr/>

<h3>Results</h3>
<p><b>Usable Energy per Cell:</b> <span id="cell_result">-</span> Wh</p>
<p><b>Module Voltage:</b> <span id="module_voltage">-</span> V</p>
<p><b>Module Capacity:</b> <span id="module_capacity">-</span> Ah</p>
<p><b>Module Energy:</b> <span id="module_energy">-</span> kWh</p>
<p><b>Pack Energy:</b> <span id="pack_energy">-</span> kWh</p>
<p><b>System Voltage:</b> <span id="system_voltage">-</span> V</p>
<p><b>System Capacity:</b> <span id="system_capacity">-</span> Ah</p>
<p><b>Total System Energy:</b> <span id="system_energy">-</span> kWh</p>
<p><b>Meets Requirement?</b> <span id="meets_req">-</span></p>
<p><b>Suggested Packs in Parallel:</b> <span id="suggest_packs_parallel">-</span></p>

<hr/>

<h3>Cost Calculation (USD | SEK | INR)</h3>
<p><b>Module Cost:</b> <span id="module_cost">-</span></p>
<p><b>Pack Cost:</b> <span id="pack_cost">-</span></p>
<p><b>Total System Cost (including container):</b> <span id="system_cost">-</span></p>

<hr/>

<h3>Visual Diagram</h3>
<div id="diagram" style="margin-top:20px;"></div>
</div>

<script>
// Currency rates
const currencyRates = { USD:1, SEK:11.5, INR:83.0 };

function setCellValues() {
  let type = document.getElementById("cell_type").value;
  let cellVoltageInput = document.getElementById("cell_voltage");
  let cellCapacityInput = document.getElementById("cell_capacity");
  let cellCostInput = document.getElementById("cell_cost");

  if(type==="NMC"){cellVoltageInput.value=3.7;cellCapacityInput.value=5;cellCostInput.value=5;}
  else if(type==="LFP"){cellVoltageInput.value=3.2;cellCapacityInput.value=100;cellCostInput.value=120;}
  else{cellVoltageInput.value="";cellCapacityInput.value="";cellCostInput.value=0;}
}

function calcBess(){
  let V=parseFloat(document.getElementById("cell_voltage").value)||0;
  let Ah=parseFloat(document.getElementById("cell_capacity").value)||0;
  let socStart=parseFloat(document.getElementById("bess_soc_start").value)||0;
  let socEnd=parseFloat(document.getElementById("bess_soc_end").value)||0;
  let eff=(parseFloat(document.getElementById("bess_eff").value)||100)/100;
  let systemKWhReq=parseFloat(document.getElementById("system_kwh").value)||0;

  let series=parseInt(document.getElementById("series_cells").value)||0;
  let parallel=parseInt(document.getElementById("parallel_cells").value)||0;
  let modulesPerPack=parseInt(document.getElementById("modules_per_pack").value)||0;

  let packsSeries=parseInt(document.getElementById("packs_series").value)||0;
  let packsParallel=parseInt(document.getElementById("packs_parallel").value)||0;

  if(socStart<=socEnd){alert("Invalid SoC range");return;}

  let usableAh=Ah*((socStart-socEnd)/100);
  let energyWhCell=V*usableAh*eff;
  let energyKWhCell=energyWhCell/1000;

  let moduleVoltage=V*series;
  let moduleCapacity=usableAh*parallel;
  let moduleEnergyWh=moduleVoltage*moduleCapacity;
  let moduleEnergyKWh=moduleEnergyWh/1000;

  let packEnergyKWh=moduleEnergyKWh*modulesPerPack;
  let packVoltage=moduleVoltage*modulesPerPack;
  let packCapacity=moduleCapacity;

  let systemVoltage=packVoltage*packsSeries;
  let systemCapacity=packCapacity*packsParallel;
  let systemEnergyKWh=packEnergyKWh*packsSeries*packsParallel;

  let minParallelPacks=Math.ceil(systemKWhReq/(packEnergyKWh*packsSeries));

  document.getElementById("cell_result").innerText=energyWhCell.toFixed(2);
  document.getElementById("module_voltage").innerText=moduleVoltage.toFixed(2);
  document.getElementById("module_capacity").innerText=moduleCapacity.toFixed(2);
  document.getElementById("module_energy").innerText=moduleEnergyKWh.toFixed(2);
  document.getElementById("pack_energy").innerText=packEnergyKWh.toFixed(2);
  document.getElementById("system_voltage").innerText=systemVoltage.toFixed(2);
  document.getElementById("system_capacity").innerText=systemCapacity.toFixed(2);
  document.getElementById("system_energy").innerText=systemEnergyKWh.toFixed(2);
  document.getElementById("meets_req").innerText=systemEnergyKWh>=systemKWhReq?"✅ Yes":"❌ No";
  document.getElementById("suggest_packs_parallel").innerText=minParallelPacks;

  calcCosts(series, parallel, modulesPerPack, packsSeries, minParallelPacks);
  drawDiagram(series, parallel, modulesPerPack, packsSeries, minParallelPacks);
}

function calcCosts(series, parallel, modulesPerPack, packsSeries, packsParallel){
  let cellCostUSD=parseFloat(document.getElementById("cell_cost").value)||0;
  let moduleExtraUSD=parseFloat(document.getElementById("module_extra_cost").value)||0;
  let packExtraUSD=parseFloat(document.getElementById("pack_extra_cost").value)||0;
  let containerUSD=parseFloat(document.getElementById("container_cost").value)||0;

  let cellsPerModule=series*parallel;
  let moduleCostUSD=cellsPerModule*cellCostUSD+moduleExtraUSD;
  let packCostUSD=moduleCostUSD*modulesPerPack+packExtraUSD;
  let systemCostUSD=packCostUSD*packsSeries*packsParallel+containerUSD;

  document.getElementById("module_cost").innerText=`$${moduleCostUSD.toFixed(2)} | ${(moduleCostUSD*currencyRates.SEK).toFixed(0)} SEK | ${(moduleCostUSD*currencyRates.INR).toFixed(0)} INR`;
  document.getElementById("pack_cost").innerText=`$${packCostUSD.toFixed(2)} | ${(packCostUSD*currencyRates.SEK).toFixed(0)} SEK | ${(packCostUSD*currencyRates.INR).toFixed(0)} INR`;
  document.getElementById("system_cost").innerText=`$${systemCostUSD.toFixed(2)} | ${(systemCostUSD*currencyRates.SEK).toFixed(0)} SEK | ${(systemCostUSD*currencyRates.INR).toFixed(0)} INR`;
}

function drawDiagram(series, parallel, modulesPerPack, packsSeries, packsParallel){
  let V=parseFloat(document.getElementById("cell_voltage").value)||0;
  let Ah=parseFloat(document.getElementById("cell_capacity").value)||0;
  let socStart=parseFloat(document.getElementById("bess_soc_start").value)||0;
  let socEnd=parseFloat(document.getElementById("bess_soc_end").value)||0;
  let eff=(parseFloat(document.getElementById("bess_eff").value)||100)/100;
  let cellCost=parseFloat(document.getElementById("cell_cost").value)||0;
  let moduleExtra=parseFloat(document.getElementById("module_extra_cost").value)||0;
  let packExtra=parseFloat(document.getElementById("pack_extra_cost").value)||0;
  let containerCost=parseFloat(document.getElementById("container_cost").value)||0;

  let usableAh=Ah*((socStart-socEnd)/100);
  let energyWhCell=V*usableAh*eff;
  let energyKWhCell=energyWhCell/1000;

  let cellsPerModule=series*parallel;
  let moduleEnergy=energyKWhCell*cellsPerModule;
  let moduleCostTotal=cellsPerModule*cellCost+moduleExtra;

  let packEnergy=moduleEnergy*modulesPerPack;
  let packCostTotal=moduleCostTotal*modulesPerPack+packExtra;

  let html='<div style="display:flex;gap:10px;">';
  for(let pPar=0;pPar<packsParallel;pPar++){
    html+=`<div style="border:2px solid #007bff;padding:5px;" title="Pack ${pPar+1} (Parallel)\nEnergy: ${(packEnergy*packsSeries).toFixed(2)} kWh\nCost: $${(packCostTotal*packsSeries+containerCost).toFixed(2)} | ${(packCostTotal*packsSeries*currencyRates.SEK+containerCost*currencyRates.SEK).toFixed(0)} SEK | ${(packCostTotal*packsSeries*currencyRates.INR+containerCost*currencyRates.INR).toFixed(0)} INR">Pack ${pPar+1} (Parallel)<br>`;
    for(let pSer=0;pSer<packsSeries;pSer++){
      html+=`<div style="border:2px solid #28a745;margin:5px;padding:5px;" title="Pack ${pSer+1} (Series)\nEnergy: ${packEnergy.toFixed(2)} kWh\nCost: $${packCostTotal.toFixed(2)} | ${(packCostTotal*currencyRates.SEK).toFixed(0)} SEK | ${(packCostTotal*currencyRates.INR).toFixed(0)} INR">Pack ${pSer+1} (Series)<br>`;
      for(let m=0;m<modulesPerPack;m++){
        html+=`<div style="border:2px solid #ffc107;margin:3px;padding:3px;" title="Module ${m+1}\nEnergy: ${moduleEnergy.toFixed(2)} kWh\nCost: $${moduleCostTotal.toFixed(2)} | ${(moduleCostTotal*currencyRates.SEK).toFixed(0)} SEK | ${(moduleCostTotal*currencyRates.INR).toFixed(0)} INR">Module ${m+1}<br>`;
        for(let cPar=0;cPar<parallel;cPar++){
          html+='<div style="display:flex;margin:1px;">';
          for(let cSer=0;cSer<series;cSer++){
            html+=`<div style="width:15px;height:15px;margin:1px;background:#dc3545;border:1px solid #000;" title="Cell Energy: ${energyKWhCell.toFixed(3)} kWh\nCost: $${cellCost.toFixed(2)} | ${(cellCost*currencyRates.SEK).toFixed(0)} SEK | ${(cellCost*currencyRates.INR).toFixed(0)} INR"></div>`;
          }
          html+='</div>';
        }
        html+='</div>';
      }
      html+='</div>';
    }
    html+='</div>';
  }
  html+='</div>';
  document.getElementById("diagram").innerHTML=html;
}
</script>
