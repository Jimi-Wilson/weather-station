import axios from 'axios';

const API_BASE_URL = import.meta.env.VITE_API_URL || 'http://localhost:8000/api';
const apiKey = import.meta.env.VITE_API_KEY;

export const apiClient = axios.create({
    baseURL: API_BASE_URL,
    headers: {
        'Content-Type': 'application/json',
        'Authorization': `Api-Key ${apiKey}`
    },
    timeout: 10000,
});

apiClient.interceptors.response.use(
    (response) => response,
    error => {
        if (error.response?.status === 401) {
            console.error('API Key authentication failed');
        }
        return Promise.reject(error);
    }
);
