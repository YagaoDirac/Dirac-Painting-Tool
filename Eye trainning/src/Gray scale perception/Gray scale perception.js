/*
 This is part of the Dirac's painting tool. Part of the eye training tool set.
 This one trains the perspection of gray scale.

To get the latest version, check out github.com/yagaodirac. 
 */

//let min = 0;
//let max = 255;
let answer = 204;


function trainingPageOnload(){ }




let steps = []
steps[0] = function (context) {

   genAnswer();

   //Noise in background.
   const blockSize = 50;
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
      let a_random_number = Math.random();
      let offset = { x: Math.random() * 360 - 180, y: Math.random() * 360 - 180 };

      if (a_random_number < 0.3) {
         let size = Math.random() * 240 + 40;

         context.fillStyle = "rgb(" + answer + "," + answer + "," + answer + ")";
         context.fillRect((canvas.width - size) / 2 + offset.x, (canvas.height - size) / 2 + offset.y, size, size);

         return;
      }
      if (a_random_number < 0.8) {
         let sizeOut = Math.random() * 170 + 110;
         let sizeCore = Math.random() * 10 + 12;

         context.lineWidth = 2;
         context.strokeStyle = "rgba(" + answer + "," + answer + "," + answer + ", 1)";
         context.strokeRect((canvas.width - sizeOut) / 2 + offset.x, (canvas.height - sizeOut) / 2 + offset.y, sizeOut, sizeOut);

         context.fillStyle = "rgb(" + answer + "," + answer + "," + answer + ")";
         context.fillRect((canvas.width - sizeCore) / 2 + offset.x, (canvas.height - sizeCore) / 2 + offset.y, sizeCore, sizeCore);

         return;
      }
      if(true)
      {
         let question_width = Math.random() * 350 + 50;
         let question_height = Math.random() * 20 + 3;
         if (Math.random() < 0.5) { [question_width, question_height] = [question_height, question_width]; }

         context.fillStyle = "rgb(" + answer + "," + answer + "," + answer + ")";
         context.fillRect((canvas.width - question_width) / 2 + offset.x, (canvas.height - question_height) / 2 + offset.y, question_width, question_height);

         return;
      }

   }

}

steps[1] = function (context) {

   let answerText = answer.toString() + " " + Math.round(answer / 2.55) + "%";
   context.textAlign = "center";
   context.textBaseline = "middle";
   context.font = "bold 50px Arial";
   {
      if (answer < 100) {
         let temp = answer + 20;
         context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
      }
      else {
         let temp = answer - 15;
         context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
      }
      context.lineWidth = 2;
      context.lineCap = "butt";
      context.lineJoin = "bevel";
   }

   let layout = {};
   layout.answerPositionTop = {};
   layout.answerPositionBottom = {};
   layout.answerPositionTop = { x: canvas.width / 2, y: 220 };
   layout.answerPositionBottom = { x: layout.answerPositionTop.x, y: canvas.height - layout.answerPositionTop.y };


   context.strokeText(answerText, layout.answerPositionTop.x, layout.answerPositionTop.y);
   context.fillText(answerText, layout.answerPositionTop.x, layout.answerPositionTop.y);
   context.strokeText(answerText, layout.answerPositionBottom.x, layout.answerPositionBottom.y);
   context.fillText(answerText, layout.answerPositionBottom.x, layout.answerPositionBottom.y);

}









function genAnswer() {
   answer = Math.random() * 256;
   if ((answer > 240 || answer < 10) && (Math.random() < 0.17)) {answer = Math.random() * 256;}
   if ((answer > 230 || answer < 20) && (Math.random() < 0.17)) { answer = Math.random() * 256;}
   if ((answer > 220 || answer < 30) && (Math.random() < 0.17)) { answer = Math.random() * 256;}
   if ((answer > 210 || answer < 45) && (Math.random() < 0.17)) { answer = Math.random() * 256;}
   if ((answer > 200 || answer < 60) && (Math.random() < 0.17)) { answer = Math.random() * 256;}
   if ((answer > 200 || answer < 80) && (Math.random() < 0.17)) { answer = Math.random() * 256; }
   answer = Math.floor(answer);
}