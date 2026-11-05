# Y.A.H.A.I.R.A. - Yielding Action & Hash-driven Animation Infrastructure for Rendering Architectures

A data-driven graphics engine built on top of dictionaries and tags.

## Overview
Y.A.H.A.I.R.A.'s core API is completely headless and renderer-agnostic. The simulation state lives in pure memory structures, meaning it can be consumed by any rendering library or graphics framework (OpenGL, Direct3D, or even Matplotlib) whether your frontend is written in C, Java, Python, or anything else.

## How it works
* **Grid-Based Space:** Defines object positioning and spatial coordinates within a unified GRID.
* **Vector Transformations:** Applies transformation functions directly over position vectors via dynamic action tags.
* **State Interpolation:** Instructs entities on how to interpolate positions smoothly across simulation steps.

> **Note:** This repository contains Y.A.H.A.I.R.A.'s MVP and represents my first long-term maintained software project.