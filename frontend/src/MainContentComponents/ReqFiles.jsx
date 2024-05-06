import { Container, Row, Col, Card, Button, Form } from "react-bootstrap"
import React, { useState } from "react"

function BasicExample({ compressed_file, decoder_map, frequency_table }) {
  // Function to download text as a file
  const downloadText = (text, filename) => {
    const element = document.createElement("a")
    const file = new Blob([text], { type: "text/plain" })
    element.href = URL.createObjectURL(file)
    element.download = filename
    document.body.appendChild(element) // Required for Firefox
    element.click()
    document.body.removeChild(element) // Clean up
  }
  const downloadBitsAsBin = (bits, filename) => {
    // Convert bits to bytes
    const bytes = []
    for (let i = 0; i < bits.length; i += 8) {
      const byte = parseInt(bits.substr(i, 8), 2)
      bytes.push(byte)
    }

    // Create a Blob with the byte array
    const blob = new Blob([new Uint8Array(bytes)], {
      type: "application/octet-stream",
    })

    // Create a download link and trigger the download
    const element = document.createElement("a")
    element.href = URL.createObjectURL(blob)
    element.download = filename
    document.body.appendChild(element) // Required for Firefox
    element.click()
    document.body.removeChild(element) // Clean up
  }

  return (
    <Container>
      <Row style={{ marginTop: "30px" }}>
        <Col xs={12} md={6} lg={3}>
          <Card style={{ width: "18rem" }}>
            <Card.Body>
              <Card.Title>Frequency Table</Card.Title>
              <Form.Control
                as="textarea"
                rows={3}
                value={frequency_table}
                readOnly
              />
              <Button
                variant="success"
                onClick={() =>
                  downloadText(frequency_table, "frequency_table.txt")
                }
              >
                Download
              </Button>
            </Card.Body>
          </Card>
        </Col>
        <Col xs={12} md={6} lg={3}>
          <Card style={{ width: "18rem" }}>
            <Card.Body>
              <Card.Title>Compressed Txt</Card.Title>
              <Form.Control
                as="textarea"
                rows={3}
                value={compressed_file}
                readOnly
              />
              <Button
                variant="success"
                onClick={() =>
                  downloadBitsAsBin(compressed_file, "compressed_txt.txt")
                }
              >
                Download
              </Button>
            </Card.Body>
          </Card>
        </Col>
        <Col xs={12} md={6} lg={3}>
          <Card style={{ width: "18rem" }}>
            <Card.Body>
              <Card.Title>Decoded Map</Card.Title>
              <Form.Control
                as="textarea"
                rows={3}
                value={decoder_map}
                readOnly
              />
              <Button
                variant="success"
                onClick={() => downloadText(decoder_map, "decoded_map.json")}
              >
                Download
              </Button>
            </Card.Body>
          </Card>
        </Col>
      </Row>
    </Container>
  )
}

export default BasicExample
