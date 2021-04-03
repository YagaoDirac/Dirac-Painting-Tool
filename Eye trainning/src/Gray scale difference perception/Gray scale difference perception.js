/*
 This is part of the Dirac's painting tool. Part of the eye training tool set.
 This one trains the counting of lines.
 暂时只有灰色的

To get the latest version, check out github.com/yagaodirac. 
 */

let min = 0; //int. At least 0.
let max = 120; //int. Better less than 63.


let answer = {};
let layout = null


function trainingPageOnload() { 

 
   layout = {};
   layout.answerPositionTop = {};
   layout.answerPositionBottom = {};
   layout.referenceBlockSize = 120;

   layout.answerPositionTop = { x: canvas.width / 2, y: 400 };
   layout.answerPositionBottom = { x: layout.answerPositionTop.x, y: canvas.height - layout.answerPositionTop.y };

   requestAnimationFrame(canvasOnPaint);

}


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
      let startPoint = [Math.cos(answer.rotation), Math.sin(answer.rotation)];
      startPoint = startPoint.map((value) => value * answer.radius);
      startPoint = { x: startPoint[0] + canvas.width / 2, y: startPoint[1] + canvas.height / 2 };
      let linearGradient = context.createLinearGradient(startPoint.x, startPoint.y, canvas.width - startPoint.x, canvas.height - startPoint.y);

      //__DEGUB__
      if (true) {
         let size = 4;
         context.fillStyle = "rgb(155,144,155)";
         context.fillRect(startPoint.x - size / 2, startPoint.y - size / 2, size, size);
      }


      answer.elementList.forEach(function (value) {
         linearGradient.addColorStop(value.stop, makeGreyColor(value.color));
      });


      context.fillStyle = linearGradient;
      context.beginPath();
      context.arc(canvas.width / 2, canvas.height / 2, answer.radius, 0, Math.PI * 2);
      context.fill();
   }
}
steps[1] = function (context) {

   //part 1 answer[0]
   let answerText = answer.answer[0].toString();
   context.textAlign = "center";
   context.textBaseline = "middle";
   context.font = "bold 50px Arial";

   context.fillStyle = makeGreyColor(answer.answer[0]);
   {
      if (answer.answer[0] < 100) {
         let temp = answer.answer[0] + 50;
         context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
      }
      else {
         let temp = answer.answer[0] - 45;
         context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
      }
      context.lineWidth = 4;
      context.lineCap = "butt";
      context.lineJoin = "bevel";
   }

   let textPos = [Math.cos(answer.rotation), Math.sin(answer.rotation)];
   textPos = textPos.map((value) => value * (answer.radius + 200));
   textPos = { x: textPos[0] + canvas.width / 2, y: textPos[1] + canvas.height / 2 };

   context.strokeText(answerText, textPos.x, textPos.y);
   context.fillText(answerText, textPos.x, textPos.y);

   //part 1 answer[1]
   answerText = answer.answer[1].toString();
   context.textAlign = "center";
   context.textBaseline = "middle";
   context.font = "bold 50px Arial";

   context.fillStyle = makeGreyColor(answer.answer[1]);
   {
      if (answer.answer[1] < 100) {
         let temp = answer.answer[1] + 50;
         context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
      }
      else {
         let temp = answer.answer[1] - 45;
         context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
      }
      context.lineWidth = 4;
      context.lineCap = "butt";
      context.lineJoin = "bevel";
   }

   textPos = [Math.cos(answer.rotation), Math.sin(answer.rotation)];
   textPos = textPos.map((value) => value * (answer.radius + 200) * -1);
   textPos = { x: textPos[0] + canvas.width / 2, y: textPos[1] + canvas.height / 2 };

   context.strokeText(answerText, textPos.x, textPos.y);
   context.fillText(answerText, textPos.x, textPos.y);

   //part 2 diff
   answerText = (answer.answer[1] - answer.answer[0]).toString();
   context.textAlign = "center";
   context.textBaseline = "middle";
   context.font = "bold 25px Arial";

   let color = Math.floor((answer.answer[0] + answer.answer[1]) / 2);
   color += 128;
   if (color > 255) color -= 256;
   context.fillStyle = "rgba(" + color + "," + color + "," + color + ",0.2)";
   {
      if (color < 100) {
         let temp = color + 50;
         context.strokeStyle = "rgba(" + temp + "," + temp + "," + temp + ",0.2)";
      }
      else {
         let temp = color - 45;
         context.strokeStyle = "rgba(" + temp + "," + temp + "," + temp + ",0.2)";
      }
      context.lineWidth = 4;
      context.lineCap = "butt";
      context.lineJoin = "bevel";
   }

   textPos = [Math.cos(answer.rotation), Math.sin(answer.rotation)];
   textPos = textPos.map((value) => value * (answer.radius + 200) * -1);
   textPos = { x: canvas.width / 2, y: canvas.height / 2 };

   context.strokeText(answerText, textPos.x, textPos.y);
   context.fillText(answerText, textPos.x, textPos.y);




   //context.strokeText(answerText, layout.answerPositionBottom.x, canvas.height / 2);
   //context.fillText(answerText, layout.answerPositionBottom.x, canvas.height / 2);


}

function genAnswer() {
   answer = {};

   answer.answer = [];
   {
      let diff = Math.floor(Math.random() * (max - (min - 1)) + min);
      if (diff > 120 && Math.random() < 0.4) { diff = Math.floor(Math.random() * (max - (min - 1)) + min) }
      if (diff > 160 && Math.random() < 0.6) { diff = Math.floor(Math.random() * (max - (min - 1)) + min) }
		


      answer.answer.push(Math.floor(Math.random() * (255 - diff)));
      answer.answer.push(Math.floor(answer.answer[0] + diff));
	}

   answer.rotation = Math.random() * Math.PI * 100;
   answer.radius = Math.random() * 70 + 20;

   answer.elementList = []
   {
      let temp = Math.random() * 0.5 + 0.25;
      answer.elementList.push({ stop: 0, color: answer.answer[0] });
      answer.elementList.push({ stop: temp, color: answer.answer[0] });
      answer.elementList.push({ stop: temp, color: answer.answer[1] });
      answer.elementList.push({ stop: 1, color: answer.answer[1] });
	}
}

/**
 * @param {Number} greyScale : int, 0 to 255
 */
function makeGreyColor(greyScale) {
   return "rgb(" + greyScale + "," + greyScale + "," + greyScale + ")";
}
/**
 * Unused in this project.
 * @param {Number} origin The Origin color
 * @param {Number} distance The distance
 */
function getDifferentColor(origin, distance) {
   //origin must be 0~255. distance has to be less than 127. But this function is not suposed to work very precisely.
   //if distance <=1, it means all the colors are ok.
   let s1 = 0;
   let e1 = origin - distance;
   let count1 = e1 - s1 + 1;
   if (count1 < 0) count1 = 0;

   let s2 = origin + distance;
   let e2 = 255;
   let count2 = e2 - s2 + 1;
   if (count2 < 0) count2 = 0;

   let totalCount = count1 + count2;
   if (0 == totalCount) alert("Function getDifferentColor failed.");
   let rd = Math.floor(Math.random() * totalCount);

   if (rd < count1) return s1 + rd;
   return s2 + (rd - count1);
}
/**
 * Unused in this project.
 * @param {Number} ori1 The Origin color 1
 * @param {Number} ori2 The Origin color 2
 * @param {Number} distance The distance
 */
function getDifferentColor2(ori1, ori2, distance) {
   if (ori1 > ori2) { [ori1, ori2] = [ori2, ori1]; }
   let s1 = 0;
   let e1 = ori1 - distance;
   let count1 = e1 - s1 + 1;
   if (count1 < 0) count1 = 0;

   let s2 = ori1 + distance;
   let e2 = ori2 - distance;
   let count2 = e2 - s2 + 1;
   if (count2 < 0) count2 = 0;

   let s3 = ori2 + distance;
   let e3 = 255;
   let count3 = e3 - s3 + 1;
   if (count3 < 0) count3 = 0;

   let totalCount = count1 + count2 + count3;
   if (0 == totalCount) {
      alert("Function getDifferentColor2 failed. The 2 version, not the 1 version. \n parameters: " + toString(ori1) + ", " + toString(ori2) + ", " + toString(distance) + ")");
      let bp = 43243214312;
   }
   let rd = Math.floor(Math.random() * totalCount + 1);//At least 1.

   if (rd <= count1) { return s1 + rd - 1 };
   rd -= count1;
   if (rd <= count2) { return s2 + rd - 1 };
   rd -= count2;
   return s3 + rd - 1;
}