/*
 This is part of the Dirac's painting tool. Part of the eye training tool set.
 This one trains the counting of lines.
 暂时只有灰色的

To get the latest version, check out github.com/yagaodirac. 
 */


let min = 7;//Included
let max = 24;//Included
let colorDifferenceBetweenNabours = 25;//Better <=62. Not very sure. If this value >65, the function getDifferentColor2(ori1, ori2, distance)has a change to fail.



let answer = {};
let layout = {};


layout.answerPositionTop = {};
layout.answerPositionBottom = {};
layout.referenceBlockSize = 240;
function trainingPageOnload() {
   layout.answerPositionTop = { x: canvas.width/2, y:200};
   layout.answerPositionBottom = { x: layout.answerPositionTop.x, y: canvas.height - layout.answerPositionTop.y };
}




let steps = []
steps[0] = function (context) {

   genAnswer();

   //Noise in background.
   const blockSize = 50;
   for (let xFragment = 0; xFragment < (canvas.width + 1) / blockSize; xFragment++)
      for (let yFragment = 0; yFragment < (canvas.height + 1) / blockSize; yFragment++) {
         let tempBright = Math.floor(Math.random() * 255);
         context.fillStyle = "rgb(" + tempBright + "," + tempBright + "," + tempBright + ")";
         context.fillRect(xFragment * blockSize, yFragment * blockSize, 100, 100);

      }
   context.fillStyle = "rgba(" + answer.backgroundColorInDot + "," + answer.backgroundColorInDot + "," + answer.backgroundColorInDot + "," + 0.4 + ")";
   context.fillRect(0, 0, canvas.width, canvas.height);

   //Draws reference block on 4 corners.
   if (false) {
      let topLeftIsWhite = true;
      if (Math.random() < 0.5) { topLeftIsWhite = false; }
      if (topLeftIsWhite) context.fillStyle = "rgb(255, 255, 255)";
      else context.fillStyle = "rgb(0, 0, 0)";
      context.fillRect(0, 0, layout.referenceBlockSize, layout.referenceBlockSize);
      context.fillRect(canvas.width - layout.referenceBlockSize, canvas.height - layout.referenceBlockSize, canvas.width, canvas.height);
      if (topLeftIsWhite) context.fillStyle = "rgb(0, 0, 0)";
      else context.fillStyle = "rgb(255, 255, 255)";
      context.fillRect(0, canvas.height - layout.referenceBlockSize, layout.referenceBlockSize, canvas.height);
      context.fillRect(canvas.width - layout.referenceBlockSize, 0, canvas.width, layout.referenceBlockSize);
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

         //print the data.
         console.clear();
         console.log(answer.elementList);
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

   let answerText = answer.answer.toString();
   context.textAlign = "center";
   context.textBaseline = "middle";
   context.font = "bold 50px Arial";


   context.fillStyle = makeGreyColor(answer.backgroundColorInDot);
   {
      if (answer.backgroundColorInDot < 100) {
         let temp = answer.backgroundColorInDot + 50;
         context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
      }
      else {
         let temp = answer.backgroundColorInDot - 45;
         context.strokeStyle = "rgb(" + temp + "," + temp + "," + temp + ")";
      }
      context.lineWidth = 4;
      context.lineCap = "butt";
      context.lineJoin = "bevel";
   }

   context.strokeText(answerText, layout.answerPositionTop.x, layout.answerPositionTop.y - (answer.radius * 0.6 - 100));
   context.fillText(answerText, layout.answerPositionTop.x, layout.answerPositionTop.y - (answer.radius * 0.6 - 100));
   context.strokeText(answerText, layout.answerPositionBottom.x, layout.answerPositionBottom.y + (answer.radius * 0.6 - 100));
   context.fillText(answerText, layout.answerPositionBottom.x, layout.answerPositionBottom.y + (answer.radius * 0.6 - 100));





   //The one in the mid.
   context.font = "bold 35px Arial";
   context.fillStyle = "rgba(" + answer.backgroundColorInDot + "," + answer.backgroundColorInDot + "," + answer.backgroundColorInDot + ",0.5)";
   {
      if (answer.backgroundColorInDot < 100) {
         let temp = answer.backgroundColorInDot + 50;
         context.strokeStyle = "rgba(" + temp + "," + temp + "," + temp + ",0.5)";
      }
      else {
         let temp = answer.backgroundColorInDot - 45;
         context.strokeStyle = "rgba(" + temp + "," + temp + "," + temp + ",0.5)";
      }
      context.lineWidth = 2;
      context.lineCap = "butt";
      context.lineJoin = "bevel";
   }


   context.strokeText(answerText, layout.answerPositionBottom.x, canvas.height / 2);
   context.fillText(answerText, layout.answerPositionBottom.x, canvas.height / 2);

}

function genAnswer() {
   answer = {};

   answer.answer = Math.floor(Math.random() * (max - min + 1) + min);
   answer.rotation = Math.random() * Math.PI * 100;
   answer.radius = Math.random() * (0 + answer.answer*5) + 30 + answer.answer*7;

   {
      let tempList = [];
      tempList.push(0);
      tempList.push(Math.random() * 8 + 5);//The width of background inside the question dot area.
      for (var i = 0; i < answer.answer; i++) {
         tempList.push(-1);//The linear gradient
         tempList.push(Math.random() * 1.8 + 0.7);//The same color. A line needs a width.
      }
      tempList.push(-1);//The linear gradient
      tempList.push(Math.random() * 8 + 5);//The width of background inside the question dot area.
      tempList = tempList.map(function (value, index, array) {
         if (value != -1) return value;
         let temp = (array[index - 1] + array[index + 1]) / Math.random()*1.3+2.2;
         if (temp > 2.5) temp = Math.random() * 1.2 + 1.2;
         return 0;
         return temp;
      });



      let accumulation = []
      let sum = tempList.reduce(function (accumulator, currentValue) {
         let added = accumulator + currentValue;
         accumulation.push(added);
         return added
      },0);

      answer.elementList = accumulation.map(function (currentValue) { return currentValue / sum; });
      answer.elementList[answer.elementList.length - 1] = 1;//Only for the accuracy.
   }

   answer.backgroundColorInDot = Math.floor(Math.random() * 256);
   let previousColor = answer.backgroundColorInDot;


   answer.elementList[0] = { stop: answer.elementList[0], color: answer.backgroundColorInDot };
   answer.elementList[1] = { stop: answer.elementList[1], color: answer.backgroundColorInDot };
   for (var i = 2; i < answer.elementList.length - 4; i += 2) {
      let color = getDifferentColor(previousColor, colorDifferenceBetweenNabours);
      answer.elementList[i] = { stop: answer.elementList[i], color: color };
      answer.elementList[i + 1] = { stop: answer.elementList[i + 1], color: color };
      previousColor = color;
   }
   let color = getDifferentColor2(previousColor, answer.backgroundColorInDot, colorDifferenceBetweenNabours);
   answer.elementList[answer.elementList.length - 4] = { stop: answer.elementList[answer.elementList.length - 4], color: color };
   answer.elementList[answer.elementList.length - 3] = { stop: answer.elementList[answer.elementList.length - 3], color: color };
   answer.elementList[answer.elementList.length - 2] = { stop: answer.elementList[answer.elementList.length - 2], color: answer.backgroundColorInDot };
   answer.elementList[answer.elementList.length - 1] = { stop: answer.elementList[answer.elementList.length - 1], color: answer.backgroundColorInDot };


   //For the very first version, this function ends here.
   if (false) return;








}

function makeGreyColor(greyScale) {
   return "rgb(" + greyScale + "," + greyScale + "," + greyScale + ")";
}
/**
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
   let rd = Math.floor(Math.random() * totalCount + 1) ;//At least 1.

   if (rd <= count1) { return s1 + rd - 1 };
   rd -= count1;
   if (rd <= count2) { return s2 + rd - 1 };
   rd -= count2;
   return s3 + rd - 1;
}










