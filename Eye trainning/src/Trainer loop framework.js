window.addEventListener("load", window_onload, false);
window.addEventListener("resize", window_onresize, false);
let canvas = null;
let can_display = true;
function window_onload() {
   for (var i = 0; i < (new Date()).getTime() % 12345; i++) {
      Math.random();
   }

   canvas = document.createElement("canvas");
   document.body.insertBefore(canvas, document.body.firstChild);
   window.addEventListener("click", canvasOnClick)
   window.addEventListener("keydown", canvasOnKeydown)
   canvas.width = 1000;
   canvas.height = 1000;
   canvas.alignItems = "center";

   step_index = 0;
   trainingPageOnload();
   
   //The first frame.
   moveCanvasToMid();
   can_display = true;
   display();
}
function canvasOnClick(event){
   display();
}
function canvasOnKeydown(event) {
   if (event.code == "Escape" || event.code == "Space"){
      display();
	}
}
function display() {
   if (can_display) {
      can_display = false;
      requestAnimationFrame(canvasOnPaint);
      let timeOut = 500;
      if (__DEBUG__) { timeOut = 100;}
      window.setTimeout(() => can_display = true, timeOut);
   }
}
function window_onresize() {
   moveCanvasToMid();
}
function moveCanvasToMid() {
   canvas.style.transform = "translate(" + (window.innerWidth - canvas.width) / 2 + "px," + (window.innerHeight - canvas.height) / 2 + "px)";
}
function canvasOnPaint()
{
   let context = canvas.getContext("2d");

   steps[step_index](context);
   step_index += 1;
   if (!steps[step_index]) { step_index = 0 };

   return;
}
