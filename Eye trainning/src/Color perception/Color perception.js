/*
 This is part of the Dirac's painting tool. Part of the eye training tool set.
 This one trains the perspection of gray scale.

To get the latest version, check out github.com/yagaodirac. 
 */



window.addEventListener("load", window_onload, false);
let canvas = null;
let data = null
let answer = null
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
   canvas.width = 1000;
   canvas.height= 1000;

   layout.answerPositionTop = { x: canvas.width/2, y:220};
   layout.answerPositionBottom = { x: layout.answerPositionTop.x, y: canvas.height - layout.answerPositionTop.y };

   requestAnimationFrame(canvasOnPaint);

   {
      let howToText = document.createElement("ul");
      document.body.appendChild(howToText);
      howToText.appendChild(document.createTextNode("Click in the question to continue."));
      let aboutText = document.createElement("ul");
      document.body.appendChild(aboutText);
      aboutText.appendChild(document.createTextNode("Color perception 1.0.0. Dirac's painting tool. Github.com/yagaodirac/Dirac-painting-tool"))
   }
}

function canvasOnClick(event)
{
    requestAnimationFrame(canvasOnPaint);
}

function canvasOnPaint()
{
    let context = canvas.getContext("2d");

    const blockSize = 50;


   switch (data.taskToDo) {
      case "question":
         data.taskToDo = "question with answer";

         answerGen();

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
            let size = Math.random() * 80 + 80;

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
            if (Math.random() < 1) {
               let size = Math.random() * 170 + 110;
               let offset = { x: Math.random() * 360 - 180, y: Math.random() * 360 - 180 };

               context.fillStyle = answer.getCSSColor();
               context.fillRect((canvas.width - size) / 2 + offset.x, (canvas.height - size) / 2 + offset.y, size, size);
            }
            else {
               //Not used yet
               let sizeOut = Math.random() * 170 + 110;
               let sizeCore = Math.random() * 10 + 12;
               let offset = { x: Math.random() * 600 - 300, y: Math.random() * 600 - 300 };

               context.lineWidth = 2;
               context.strokeStyle = answer.getCSSColor();
               context.strokeRect((canvas.width - sizeOut) / 2 + offset.x, (canvas.height - sizeOut) / 2 + offset.y, sizeOut, sizeOut);

               context.fillStyle = answer.getCSSColor();
               context.fillRect((canvas.width - sizeCore) / 2 + offset.x, (canvas.height - sizeCore) / 2 + offset.y, sizeCore, sizeCore);
				}
         }
         break;
      case "question with answer":

         data.taskToDo = "question";
         let answerText = answer.getHSVString() + " / " + answer.data.join(' ');
         let answerVolume = Math.max(...answer.data);
         context.textAlign = "center";
         context.textBaseline = "middle";
         context.font = "bold 60px Arial";
         {
            if (answerVolume < 100) {
               let temp = answerVolume + 20;
               context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
            }
            else {
               let temp = answerVolume - 15;
               context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
            }
            context.lineWidth = 4;
            context.lineCap = "butt";
            context.lineJoin = "bevel";
         }
         let offset = 150;

         context.strokeText(answerText, layout.answerPositionTop.x + offset, layout.answerPositionTop.y);
         context.fillText(answerText, layout.answerPositionTop.x + offset, layout.answerPositionTop.y);
         context.strokeText(answerText, layout.answerPositionBottom.x + offset, layout.answerPositionBottom.y);
         context.fillText(answerText, layout.answerPositionBottom.x + offset, layout.answerPositionBottom.y);
         break;

      default:
         alert("data.taskToDo was assigned with a wrong value.");

   }


}



function answerGen() {
   answer = djtColorRGB.random_style_2();
}


class djtColorRGB {
   /**
    * 
    * @param {number} r : Integer [0, 255] recommended
    * @param {number} g : Integer [0, 255] recommended
    * @param {number} b : Integer [0, 255] recommended
    */
   constructor(r, g, b) {
      this.data = [];
      this.data.length = 3;
      this.data[0] = r;
      this.data[1] = g;
      this.data[2] = b;
	}

   static random() {
      return new djtColorRGB(Math.floor(Math.random() * 256), Math.floor(Math.random() * 256), Math.floor(Math.random() * 256));
   }
   static random_style_2() {
      let data = [];
      data.length = 3;
      data[0] = Math.random() * 0.3 + 0.7;
      data[1] = Math.random() * 0.3;
      data[2] = Math.random() * (data[0] - data[1]) + data[1];

      if (Math.random() < 0.5) {
         [data[0], data[1]] = [data[1], data[0]];
      }
      let rand = Math.random() * 3;
      if (rand < 1) {
         data = [data[1], data[2], data[0]];
      }
      else if (rand < 2) {
         data = [data[2], data[0], data[1]];
		}

      return new djtColorRGB(Math.floor(data[0] * 256), Math.floor(data[1] * 256), Math.floor(data[2] * 256));
   }


   __correct() {
      let v;
      for (var i = 0; i < 3; i++) {
         v = this.data[i];
         v = Math.floor(v);
         v = v < 0 ? 0 : (v > 255 ? 255 : v);
         this.data[i] = v;
      }
	}
   getCSSColor() {
   //   let temp = this.construct(this.r, this.g, this.b, this.a);
   //   temp.correct();
   //   return "rgba(" + temp.r + "," + temp.g + "," + temp.b + "," + temp.a + ")";
   //}
   //getCSSColorWithoutAlpha() {
      let temp = new djtColorRGB(this.data[0], this.data[1], this.data[2]);
      temp.__correct();
      return "rgb(" + temp.data.join(",") + ")";
	}

   getHSVString() {
      let temp = new djtColorRGB(...this.data);
      temp.__correct();
      let r, g, b;
      [r, g, b] = temp.data;

      let h, s, v;
      let maxIsR = true;
      let maxIsG = true;
      let maxIsB = true;
      if (r < g) maxIsR = false; else maxIsG = false;
      if (g < b) maxIsG = false; else maxIsB = false;
      if (r < b) maxIsR = false; else maxIsB = false;

      let max = Math.max(...temp.data);
      let min;
      min = Math.min(...temp.data);
      let diff = max - min;
      if (0 == diff) {
         return "0 0 " + temp.data[0];
      }

      s = Math.round(diff / max * 100);
      if (maxIsR) {
         h = 60 * ((g - b) / diff);
         if (h < 0) h += 360;
      }
      if (maxIsG) { h = 60 * ((b - r) / diff + 2) };
      if (maxIsB) { h = 60 * ((r - g) / diff + 4) };
      h = Math.round(h);

      v = Math.round(max * 100.0 / 255);
      return h.toString() + " " + s.toString() + " " + v.toString();
	}
}