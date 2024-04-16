import React, { useState } from 'react';
import { Form, Button } from 'react-bootstrap';

const MyForm = ({ onDataUpdate }) => {
  const handleSubmit = (event) => {
    event.preventDefault();
    const target = event.target;
    const buttonClicked = target.querySelector('button[type="submit"]:focus');

    if (buttonClicked.name === 'fileSubmit') {
      // Handle file submit
      const fileInput1 = target.querySelector('#compressedFile');
      const fileInput2 = target.querySelector('#decoderFile');
      const files = [fileInput1, fileInput2].map(input => input.files[0]);

      const readFiles = files.map(file => {
        return new Promise((resolve, reject) => {
          const reader = new FileReader();
          reader.onload = (e) => resolve(e.target.result);
          reader.onerror = (e) => reject(e);
          reader.readAsText(file);
        });
      });

      Promise.all(readFiles)
        .then(fileContents => {
          const data = {
            compressedFile: fileContents[0],
            decoderFile: fileContents[1],
          };
          console.log('Data to send:', data);
          sendDataToServer(data);
        })
        .catch(error => console.error('Error reading files:', error));
    }
  };

  const sendDataToServer = (data) => {
    const url = 'http://localhost:8080/upload';
    fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(data),
    })
      .then(response => response.json())
      .then(data => {
        console.log('Success:', data);
        onDataUpdate(data);
      })
      .catch((error) => {
        console.error('Error:', error);
      });
  };

  return (
    <div style={{ display: "flex", justifyContent: "space-between", }}>
      <Form onSubmit={handleSubmit} style={{ display: 'flex', flexDirection: "column", alignItems: 'center', justifyContent: 'flex-start' }}>
        <Form.Group className="mb-3" style={{ marginRight: '10px', marginLeft: "10px" }}>
          <Form.Label style={{ fontSize: "14px" }}>Compressed File:</Form.Label>
          <Form.Control type="file" style={{ fontSize: "14px" }} id="compressedFile" />
        </Form.Group>

        <Form.Group className="mb-3" style={{ marginRight: '10px', marginLeft: "10px" }}>
          <Form.Label style={{ fontSize: "14px" }}>Decoder Map:</Form.Label>
          <Form.Control type="file" style={{ fontSize: "14px" }} id="decoderFile" />
        </Form.Group>


        <Button variant="outline-light" type="submit" name="fileSubmit" size="sm" style={{ marginBottom: "7px", fontSize: "14px" }}>Submit Files</Button>
      </Form>
    </div>
  );
};

export default MyForm;
