import React from 'react';
import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';

import 'bootstrap/dist/css/bootstrap.min.css';
import styles from './index.module.css';
import '../css/custom.css'

const Hero = () => {
  return <Container>
    <Row>
      <Col xs={12} md={4}>
        <h1 style={{ marginBottom: '0px', fontSize: '68px', fontWeight: '100', marginTop: '40px' }}>Laser Tag OS</h1>
        <h2>Open Source laser tag system</h2>
        <a href="https://github.com/NilSkilz/tag-os">https://github.com/NilSkilz/tag-os</a>
        <div style={{ marginTop: '20px' }}>
          <ul>
            <li>
              3D Printed design
            </li>
            <li>
              Custom PCB
            </li>
            <li>
              Based on ESP32
            </li>
            <li>
              TFT screen for ammo/health
            </li>
            <li>
              Onboard sound effects
            </li>
            <li>
              Rail mount system for custom sights/scopes
            </li>
          </ul>

        </div>
      </Col>
      <Col xs={12} md={8}>
        <div>
          <img src="/tag-os/img/tagger.png" width="auto" />
        </div>
      </Col>
    </Row>
    <Row>
      <Col sx={12} md={4}>
        <div>
          <img src="/tag-os/img/Gun_V4_2023-Aug-06_04-37-01PM-000_CustomizedView58838849675.png" width="auto" />
        </div>
      </Col>
      <Col sx={12} md={4}>
        <div>
          <img src="/tag-os/img/Gun_V4_2023-Aug-06_04-31-38PM-000_CustomizedView7020427754.png" width="auto" />
        </div>
      </Col>
      <Col sx={12} md={4}>
        <div>
          <img src="/tag-os/img/Gun_V4_2023-Aug-06_04-38-15PM-000_CustomizedView66498796618.png" width="auto" />
        </div>
      </Col>
    </Row>
  </Container>
}

function HomepageHeader() {
  const { siteConfig } = useDocusaurusContext();
  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)}>
      <div className="container">
        <h1 className="hero__title">{siteConfig.title}</h1>
        <p className="hero__subtitle">{siteConfig.tagline}</p>
        <div className={styles.buttons}>
          <Link
            className="button button--secondary button--lg"
            to="/docs/intro">
            Docusaurus Tutorial - 5min ⏱️
          </Link>
        </div>
      </div>
    </header>
  );
}

export default function Home() {
  const { siteConfig } = useDocusaurusContext();
  return (
    <Layout
      title={`TAG/OS`}
      description="Open Source Laser Tag">
      <Hero />
      {/* <HomepageHeader />
      <main>
        <HomepageFeatures />
      </main> */}
    </Layout>
  );
}
