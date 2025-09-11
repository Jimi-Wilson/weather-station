import axios from 'axios';

const API_BASE_URL = import.meta.env.VITE_API_URL === undefined ? '/api' : import.meta.env.VITE_API_URL;

export const apiClient = axios.create({
    baseURL: API_BASE_URL,
    headers: {
        'Content-Type': 'application/json',
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
