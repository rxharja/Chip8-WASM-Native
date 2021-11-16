
import React from 'react';
import '../Chip8.css'

const Computer = (props) => {

  return (
    <div className="macintosh" aria-label="1984 Macintosh illustration">

      <div className="monitor">
        <div className="monitor-inner">
          <div className="screen-cutout">
            <div className="screen">
              {props.children}
            </div>
          </div>
          <div className="logo">
          </div>
          <div className="opening">
            <div className="opening-inner">
            </div>
          </div>
        </div>
      </div>

      <div className="foot">
        <div className="inset"></div>
        <div className="cable-container">
          <div className="cable-hole"></div>
        </div>
      </div>

    </div>
  );
};

export default Computer;
