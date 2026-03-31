// Mobile Controller HTML (compressed)
// Insert this file in Dumb_Little_Robot_V1.ino (before setup())

const char MOBILE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0,user-scalable=no">
<meta name="apple-mobile-web-app-capable" content="yes">
<title>TinyDeskBot</title>
<style>
*{margin:0;padding:0;box-sizing:border-box;-webkit-tap-highlight-color:transparent;user-select:none;-webkit-user-select:none}
body{font-family:-apple-system,BlinkMacSystemFont,sans-serif;background:#0F172A;color:#F1F5F9;min-height:100vh;display:flex;flex-direction:column;overflow-x:hidden}
.header{background:#1E293B;padding:20px;text-align:center;border-bottom:2px solid #3B82F6;display:flex;justify-content:space-between;align-items:center}
.header h1{font-size:24px;background:linear-gradient(135deg,#3B82F6,#8B5CF6);-webkit-background-clip:text;-webkit-text-fill-color:transparent;flex:1}
.toggle{background:#334155;border:none;padding:8px 12px;border-radius:8px;color:#94A3B8;font-size:12px;cursor:pointer;transition:all 0.2s}
.toggle.on{background:linear-gradient(135deg,#3B82F6,#2563EB);color:white}
.status{padding:12px 20px;text-align:center;font-size:14px;font-weight:600;border-left:4px solid}
.status.ok{background:rgba(16,185,129,0.15);border-left-color:#10B981;color:#10B981}
.status.err{background:rgba(239,68,68,0.15);border-left-color:#EF4444;color:#EF4444}
.ctrl{flex:1;display:flex;flex-direction:column;justify-content:center;padding:20px;max-width:600px;margin:0 auto;width:100%}
.pad{display:grid;grid-template-columns:1fr 1fr 1fr;grid-template-rows:1fr 1fr 1fr;gap:16px;margin-bottom:20px}
.btn{aspect-ratio:1;border:none;border-radius:50%;font-size:36px;background:linear-gradient(145deg,#334155,#1E293B);color:white;cursor:pointer;transition:all 0.15s;touch-action:manipulation;display:flex;align-items:center;justify-content:center;box-shadow:0 4px 20px rgba(0,0,0,0.5)}
.btn:active{transform:scale(0.92);background:linear-gradient(145deg,#3B82F6,#2563EB);box-shadow:0 0 30px rgba(59,130,246,0.4)}
.btn:nth-child(1){grid-column:2;grid-row:1}
.btn:nth-child(2){grid-column:1;grid-row:2}
.btn:nth-child(3){grid-column:3;grid-row:2}
.btn:nth-child(4){grid-column:2;grid-row:3}
.spd{background:#1E293B;padding:20px;border-radius:16px;margin-bottom:20px}
.spd label{display:block;margin-bottom:12px;font-size:15px;color:#94A3B8}
.spd input{width:100%;height:8px;border-radius:4px;background:rgba(255,255,255,0.1);outline:none;-webkit-appearance:none}
.spd input::-webkit-slider-thumb{-webkit-appearance:none;width:28px;height:28px;border-radius:50%;background:linear-gradient(135deg,#3B82F6,#2563EB);cursor:pointer;box-shadow:0 2px 12px rgba(59,130,246,0.4)}
.val{text-align:center;margin-top:12px;font-size:32px;font-weight:700;background:linear-gradient(135deg,#3B82F6,#8B5CF6);-webkit-background-clip:text;-webkit-text-fill-color:transparent}
.log{position:fixed;bottom:0;left:0;right:0;background:#1E293B;border-top:2px solid #3B82F6;max-height:40vh;overflow-y:auto;transition:transform 0.3s;transform:translateY(100%)}
.log.show{transform:translateY(0)}
.log-header{padding:12px 20px;background:#0F172A;display:flex;justify-content:space-between;align-items:center;border-bottom:1px solid #334155}
.log-header h3{font-size:14px;color:#94A3B8}
.log-clear{background:#EF4444;border:none;padding:6px 12px;border-radius:6px;color:white;font-size:12px;cursor:pointer}
.log-entry{padding:8px 20px;border-bottom:1px solid #334155;font-size:12px;font-family:monospace}
.log-entry.send{color:#3B82F6}
.log-entry.recv{color:#10B981}
.log-entry.err{color:#EF4444}
</style>
</head>
<body>
<div class="header">
<h1>🤖 TinyDeskBot</h1>
<button class="toggle" id="logtoggle" onclick="toggleLog()">📋 Log</button>
</div>
<div class="status err" id="s">⚠️ Connecting...</div>
<div class="ctrl">
<div class="pad">
<button class="btn" ontouchstart="t(event,'f')" ontouchend="t(event,'s')" onmousedown="hold('f')" onmouseup="release()">⬆️</button>
<button class="btn" ontouchstart="t(event,'l')" ontouchend="t(event,'s')" onmousedown="hold('l')" onmouseup="release()">⬅️</button>
<button class="btn" ontouchstart="t(event,'r')" ontouchend="t(event,'s')" onmousedown="hold('r')" onmouseup="release()">➡️</button>
<button class="btn" ontouchstart="t(event,'b')" ontouchend="t(event,'s')" onmousedown="hold('b')" onmouseup="release()">⬇️</button>
</div>
<div class="spd">
<label>⚡ Speed</label>
<input type="range" id="sp" min="50" max="255" value="150" oninput="document.getElementById('v').textContent=this.value">
<div class="val" id="v">150</div>
</div>
</div>
<div class="log" id="logpanel">
<div class="log-header">
<h3>📋 Log</h3>
<button class="log-clear" onclick="clearLog()">Clear</button>
</div>
<div id="logcontent"></div>
</div>
<script>
const st=document.getElementById('s');
const logPanel=document.getElementById('logpanel');
const logContent=document.getElementById('logcontent');
const logToggle=document.getElementById('logtoggle');
let logEnabled=false;
let holdInterval=null;
function t(e,cmd){
  e.preventDefault();
  if(cmd==='s') release();
  else hold(cmd);
}
function hold(cmd){
  c(cmd);
  if(holdInterval) clearInterval(holdInterval);
  holdInterval=setInterval(()=>c(cmd),100);
}
function release(){
  if(holdInterval){clearInterval(holdInterval);holdInterval=null;}
  c('s');
}
function toggleLog(){
logEnabled=!logEnabled;
logPanel.classList.toggle('show',logEnabled);
logToggle.classList.toggle('on',logEnabled);
logToggle.textContent=logEnabled?'📋 Log ✓':'📋 Log';
}
function clearLog(){
logContent.innerHTML='';
}
function addLog(msg,type){
if(!logEnabled)return;
const t=new Date().toLocaleTimeString('en-US');
const e=document.createElement('div');
e.className='log-entry '+type;
e.textContent=t+' | '+msg;
logContent.appendChild(e);
logContent.scrollTop=logContent.scrollHeight;
if(logContent.children.length>50)logContent.removeChild(logContent.firstChild);
}
function c(cmd){
const m=cmd+' '+document.getElementById('sp').value+' yes no';
addLog('→ '+m,'send');
fetch('/cmd',{method:'POST',body:m})
.then(r=>{
if(r.ok){
st.className='status ok';
st.textContent='✅ Connected';
addLog('← OK','recv');
}else{
addLog('← Error: '+r.status,'err');
}
})
.catch(e=>{
st.className='status err';
st.textContent='⚠️ Error';
addLog('← '+e.message,'err');
});
}
setTimeout(()=>c('s'),500);
</script>
</body>
</html>
)rawliteral";
