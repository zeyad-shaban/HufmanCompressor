import { Container, Row, Col, Card,Button,Form } from 'react-bootstrap';
import React, { useState } from 'react';
import MyForm2 from './MyForm2';
const Compress = () => {
  return (
    <Container fluid>
    <Row  style={{marginTop:"30px"}}>
        <Col xs={12} md={6} lg={3}>
            <Card style={{ width: '18rem' }}>
                <Card.Body>
                    <Card.Title>upload files:</Card.Title>
                    <MyForm2/>
                </Card.Body>
            </Card>
        </Col>
        <Col xs={12} md={6} lg={3}>
            <Card style={{ width: '18rem' }}>
                <Card.Body>
                    <Card.Title>DeCompressed file</Card.Title>
                </Card.Body>
                <Form.Control as="textarea" rows={3} readOnly/>
                            <Button variant="success">Download</Button>
            </Card>
        </Col>
    </Row>
    
</Container>
  )
}

export default Compress