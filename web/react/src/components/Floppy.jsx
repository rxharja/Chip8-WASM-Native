import React from 'react';

const Floppy = (props) => {

    let render = props.callback;

    const games = [
        "15PUZZLE", "BLINKY", "BLITZ", "BRIX",
        "CONNECT4", "GUESS", "HIDDEN", "INVADERS",
        "KALEID", "MAZE", "MERLIN", "MISSILE",
        "PONG", "PONG2", "PUZZLE", "SYZYGY",
        "TANK", "TETRIS", "TICTAC", "UFO",
        "VBRIX", "VERS", "WIPEOFF"
    ]

    return (
        
        <div id="games">
            {
                games.map((game, i) => {
                    return (
                        <div className="floppy" id={game} key={i} onClick={(event) => render(event)}>
                            <div className="chip"></div>
                            <div className="arrow"></div>
                            <div className="note">{game}</div>
                            <div className="holes"></div>
                        </div>
                    )
                })
            }
        </div>
    );
}

export default Floppy;