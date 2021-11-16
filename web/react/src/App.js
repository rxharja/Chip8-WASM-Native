import React, { useState, useEffect } from 'react';
import Computer from "./components/Computer"
import Floppy from "./components/Floppy"
import Canvas from "./components/Canvas"


function App() {

  const [game, setGame] = useState("");

  const getId = (event) => {
    setGame(event.target.id);
  }

  useEffect(() => {
    console.log("Selected Floppy: ", game);
  }, [game]);

  return (
    <div className="App">
      <Computer>
        <Canvas game={game}/>
      </Computer>
      <Floppy callback={getId} />
    </div>
  );
}

export default App;
