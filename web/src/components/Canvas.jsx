import React, { useState, useEffect } from 'react';
import Chip8 from "../Chip8.js";
import "../Chip8.css";

const Canvas = (props) => {
  const [game, setGame] = useState(props.game);

  const loadWASM = async () => {
    let mod = {
      arguments: [`./bin/c8games/${game}`],
      preRun: [],
      postRun: [],
      // onRuntimeInitialized: function() {
      //       console.log(mod.ccall('load_file', 'number', ['string'], `./bin/c8games/PONG2`))},
      canvas: (function() {
        var canvas = document.getElementById(`canvas`);
        return canvas;
      })(),
      doNotCaptureKeyboard: true
    };

    Chip8(mod);
  }

  useEffect(() => {
    if (props.game !== "./" || props.game !== "" || props.game !== "./bin/c8games/") {
      setGame(props.game);
      loadWASM();
    }
  }, [game]);

  return (
    <canvas id="canvas" className="canvas" />
  );
}

export default Canvas;
