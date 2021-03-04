/*
 This is part of the Dirac's painting tool. Part of the eye training tool set.
 This one trains the perspection of gray scale.

To get the latest version, check out github.com/yagaodirac. 
 */

let min = 0;
let max = 255;


window.addEventListener("load", window_onload, false);
let canvas = null;
let data = null
let layout = null
function window_onload()
{
   let arr = [];
   //arr.length = 100;
   for (var i = 0; i < 100; i++) {
      arr.push(Math.random());
      //arr.push(window.crypto.getRandomValues(arr));
   }

   for (var i = 0; i < 12345; i++) {
      Math.random();
   }


   data = {};
   data.taskToDo = "question";
   data.answer = 0;
   layout = {};
   layout.answerPositionTop = {};
   layout.answerPositionBottom = {};
   //layout.referenceBlockSize = 240;

   canvas = document.createElement("canvas");
   document.body.appendChild(canvas);
   canvas.addEventListener("click", canvasOnClick)
   canvas.width = 1800;
   canvas.height= 1800;

   layout.answerPositionTop = { x: canvas.width/2, y:400};
   layout.answerPositionBottom = { x: layout.answerPositionTop.x, y: canvas.height - layout.answerPositionTop.y };

   requestAnimationFrame(canvasOnPaint);

   {
      let howToText = document.createElement("ul");
      document.body.appendChild(howToText);
      howToText.appendChild(document.createTextNode("Click in the question to continue."));
      let aboutText = document.createElement("ul");
      document.body.appendChild(aboutText);
      aboutText.appendChild(document.createTextNode("Gray scale perception 1.0.4. Dirac's painting tool. Github.com/yagaodirac/Dirac-painting-tool"))
   }
}

function canvasOnClick(event)
{
    requestAnimationFrame(canvasOnPaint);
}

function canvasOnPaint()
{
    let context = canvas.getContext("2d");

    const blockSize = 100;


   switch (data.taskToDo) {
      case "question":
         data.taskToDo = "question with answer";

         data.answer = Math.floor(Math.random() * (max-min) + min);

         //Noise in background.
         for (let xFragment = 0; xFragment < (canvas.width + 1) / blockSize; xFragment++)
            for (let yFragment = 0; yFragment < (canvas.height + 1) / blockSize; yFragment++) {
               if (Math.random() < 0.32) {
                  context.fillStyle = "hsl(" + Math.random() * 360 + "," + (Math.random() * 30 + 30) + "%," + (Math.random() * 60 + 20) + "%)";
                  context.fillRect(xFragment * blockSize, yFragment * blockSize, 100, 100);
               }
               else {
                  let tempBright = Math.floor(Math.random() * 255);
                  context.fillStyle = "rgb(" + tempBright + "," + tempBright + "," + tempBright + ")";
                  context.fillRect(xFragment * blockSize, yFragment * blockSize, 100, 100);
					}
            }

         //Draws reference block on 4 corners.
         {
            let size = Math.random() * 150 + 150;

            let topLeftIsWhite = true;
            if (Math.random() < 0.5) { topLeftIsWhite = false; }
            if (topLeftIsWhite) context.fillStyle = "rgb(255, 255, 255)";
            else context.fillStyle = "rgb(0, 0, 0)";
            context.fillRect(0, 0, size, size);
            context.fillRect(canvas.width - size, canvas.height - size, canvas.width, canvas.height);
            if (topLeftIsWhite) context.fillStyle = "rgb(0, 0, 0)";
            else context.fillStyle = "rgb(255, 255, 255)";
            context.fillRect(0, canvas.height - size, size, canvas.height);
            context.fillRect(canvas.width - size, 0, canvas.width, size);
         }

         //Draws the question dot.
         {
            if (Math.random() < 0.8) {
               let size = Math.random() * 300 + 200;
               let offset = { x: Math.random() * 600 - 300, y: Math.random() * 600 - 300 };

               context.fillStyle = "rgb(" + data.answer + "," + data.answer + "," + data.answer + ")";
               context.fillRect((canvas.width - size) / 2 + offset.x, (canvas.height - size) / 2 + offset.y, size, size);
            }
            else {
               let sizeOut = Math.random() * 300 + 200;
               let sizeCore = Math.random() * 10 + 15;
               let offset = { x: Math.random() * 600 - 300, y: Math.random() * 600 - 300 };

               context.lineWidth = 2;
               context.strokeStyle = "rgba(" + data.answer + "," + data.answer + "," + data.answer + ", 1)";
               context.strokeRect((canvas.width - sizeOut) / 2 + offset.x, (canvas.height - sizeOut) / 2 + offset.y, sizeOut, sizeOut);

               context.fillStyle = "rgb(" + data.answer + "," + data.answer + "," + data.answer + ")";
               context.fillRect((canvas.width - sizeCore) / 2 + offset.x, (canvas.height - sizeCore) / 2 + offset.y, sizeCore, sizeCore);
				}
         }
         break;
      case "question with answer":

         data.taskToDo = "question";
         let answerText = data.answer.toString() + " " + Math.round(data.answer / 2.55) + "%";
         context.textAlign = "center";
         context.textBaseline = "middle";
         context.font = "bold 100px Arial";
         {
            if (data.answer < 100) {
               let temp = data.answer + 20;
               context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
            }
            else {
               let temp = data.answer - 15;
               context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
            }
            context.lineWidth = 4;
            context.lineCap = "butt";
            context.lineJoin = "bevel";
         }
         context.strokeText(answerText, layout.answerPositionTop.x, layout.answerPositionTop.y);
         context.fillText(answerText, layout.answerPositionTop.x, layout.answerPositionTop.y);
         context.strokeText(answerText, layout.answerPositionBottom.x, layout.answerPositionBottom.y);
         context.fillText(answerText, layout.answerPositionBottom.x, layout.answerPositionBottom.y);
         break;

      default:
         alert("data.taskToDo was assigned with a wrong value.");

   }


}
