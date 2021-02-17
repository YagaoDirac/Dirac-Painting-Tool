/*
 This is part of the Dirac's painting tool. Part of the eye training tool set.
 This one trains the perspection of gray scale.

To get the latest version, check out github.com/yagaodirac. 
 */

window.addEventListener("load", window_onload, false);
let canvas = null;
let data = null
let layout = null
function window_onload()
{
    data = {};
    data.taskToDo = "question";
    data.answer = 0;
    layout = {};
    layout.answerPositionTop = {};
    layout.answerPositionBottom = {};
    layout.referenceBlockSize = 240;

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
        howToText.appendChild(document.createTextNode("Click in the question to continue."))
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

            data.answer = Math.floor(Math.random() * 255);

            //Noise in background.
            for (let xFragment = 0; xFragment < (canvas.width + 1) / blockSize; xFragment++)
                for (let yFragment = 0; yFragment < (canvas.height + 1) / blockSize; yFragment++) {
                    let tempBright = Math.floor(Math.random() * 255);
                    context.fillStyle = "rgb(" + tempBright + "," + tempBright + "," + tempBright + ")";
                    context.fillRect(xFragment * blockSize, yFragment * blockSize, 100, 100);

                }

            //Draws reference block on 4 corners.
            {
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
                let size = 300;


                context.fillStyle = "rgb(" + data.answer + "," + data.answer + "," + data.answer + ")";
                context.fillRect((canvas.width - size) / 2, (canvas.height - size) / 2, size, size);
            }
            break;
        case "question with answer":

            data.taskToDo = "question";
            let answerText = data.answer.toString() + " ( " + Math.round(data.answer / 2.55) + " % )";
            context.textAlign = "center";
            context.textBaseLine = "middle";
            context.font = "bold 100px Arial";
            context.fillText(answerText, layout.answerPositionTop.x, layout.answerPositionTop.y);
            context.fillText(answerText, layout.answerPositionBottom.x, layout.answerPositionBottom.y);
            break;

        default:
            alert("data.taskToDo was assigned with a wrong value.");

    }


}
